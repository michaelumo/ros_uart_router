/**

Payload structure
  uint8_t [header: 2 byte, length of payload: 1 byte, payload: max 253 bytes, checksum: 2 bytes]

**/

#include <iostream>
#include <cmath>
#include <cstdint>
#include <vector>
#include "ros_uart_router/BufferOperation.hpp"
#include "ros_uart_router/crc16.hpp"

BufferOperation::BufferOperation(){
  crc16Init();
}

// Clearing buffer length
void BufferOperation::clearBuffer(){
  data.erase(data.begin(), data.end());
}

// Spliting arrays
void BufferOperation::split(std::vector<uint8_t> &d, std::vector<uint8_t> &other, uint8_t a, uint8_t b){
  for(uint8_t i = a; i < b; i++){
    other.push_back(d[i]);
  }
}

// Get length of payload embedded in buffer
uint8_t BufferOperation::lengthofPayload(){
  std::vector<uint8_t> cs;
  split(data, cs, 2, 3);
  return uint8_t(cs[0]+1);
}

// Generate checksum from received buffer
uint16_t BufferOperation::generateChecksum(uint8_t starting_point, uint8_t length_of_checksum){
  std::vector<uint8_t> cs;
  split(data, cs, starting_point, data.size()-length_of_checksum);
  for(int i = 0; i < cs.size(); i++){
    std::cout<<std::hex<<(int)cs[i]<<std::endl;
  }
  return crc16(cs.data(), cs.size());//lengthofPayload());
}

// Get checksum embedded in buffer
uint16_t BufferOperation::getChecksum(){
  cksm.in8[0] = data[data.size()-2];
  cksm.in8[1] = data[data.size()-1];
  return cksm.in16;
}
