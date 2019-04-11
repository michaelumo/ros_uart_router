#include <iostream>
#include <cmath>
#include "ros_uart_router/BufferOperation.hpp"


BufferOperation::BufferOperation(){
}


void BufferOperation::clearBuffer(){
  datalen = 0;
}

void BufferOperation::split(uint8_t* d, uint8_t* other, int a, int b){
  for(int i = a; i < b; i++){
    other[i-a] = d[i];
  }
}

int BufferOperation::sizeofPayload(){
  uint8_t cs[1];
  split(data, cs, 2, 3);
  return int(cs[0]);
}

int BufferOperation::getChecksum(){
  int num = datalen-(sizeofPayload()+3)-1;//if int then 4, or if long long then 8
  uint8_t cs[num];
  int s = 0;
  split(data, cs, datalen-num, datalen);
  for(int i = 0; i<num; i++){
    int tmp = cs[i];
    s += tmp<<((num-1-i)*8);
  }
  return s;
}

int BufferOperation::sum(uint8_t* c, int a){
  int d = 0;
  for(int i = a; i < a; i++){
    d += int(c[i]);
  }
  return d;
}

int BufferOperation::sumofPayload(int a){
  int d = 0;
  for(int i = a; i < sizeofPayload()+a; i++){
    d += int(data[i]);
  }
  return d;
}

float BufferOperation::byte2float(int a, int b){
  uint8_t cs[b-a];
  split(data, cs, a, b);
  return (float)sum(cs, b-a);
}
