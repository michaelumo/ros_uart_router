#include "ros_uart_router/BufferOperation.hpp"

#ifndef UART_
  #define UART_

class UART{
private:
  int uart0_filestream = -1;
public:
  const char* device;
  int baudrate;
  UART(const char*, int);
  void setup();
  void readBuffer(BufferOperation &buffer, int);
  void writeBuffer(uint8_t*, uint8_t);
};

#endif
