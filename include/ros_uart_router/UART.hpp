#include "BufferOperation.hpp"

#ifndef UART_
  #define UART_

class UART{
private:
  int uart0_filestream = -1;
public:
  const char* device;
  uint32_t baudrate;
  UART(const char*, uint32_t);
  bool setup();
  void readBuffer(BufferOperation &buffer, int timeOut);
  void writeBuffer(unsigned char*, uint8_t);
  void closeDevice();
};

#endif
