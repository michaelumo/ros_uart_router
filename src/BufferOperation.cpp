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
  data.clear();
}

// Spliting arrays
void BufferOperation::split(std::vector<uint8_t> &d, uint8_t* other, uint8_t a, uint8_t b){
  for(uint8_t i = a; i < b; i++){
    other[i-a] = d[i];
  }
}

// Get length of payload embedded in buffer
uint8_t BufferOperation::lengthofPayload(){
  uint8_t cs[1];
  split(data, cs, 2, 3);
  return uint8_t(cs[0]+1);
}

// Generate checksum from received buffer
uint16_t BufferOperation::generateChecksum(uint8_t starting_point, uint8_t length_of_checksum){
  uint8_t cs[lengthofPayload()];
  split(data, cs, starting_point, data.size()-length_of_checksum);
  return crc16(cs, lengthofPayload());
}

// Get checksum embedded in buffer
uint16_t BufferOperation::getChecksum(){
  cksm.in8[0] = data[data.size()-2];
  cksm.in8[1] = data[data.size()-1];
  return cksm.in16;
}
