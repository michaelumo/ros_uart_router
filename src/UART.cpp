#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <termios.h>
#include <ctime>
#include <cstdint>
#include "ros/ros.h"
#include "ros_uart_router/UART.hpp"
#include "ros_uart_router/BufferOperation.hpp"

UART::UART(const char* dev, uint32_t bd){
  device = dev;
  baudrate = bd;
}

bool UART::setup(){
  uart0_filestream = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (uart0_filestream == -1)
  {
          //ERROR - CAN'T OPEN SERIAL PORT
          //std::cout << "Error - Unable to open UART.  Ensure it is not in use by another application\n" << std::endl;
          return false;
  }
  struct termios options;
  tcgetattr(uart0_filestream, &options);
  switch(baudrate){
    case 9600:
      options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
      break;
    case 115200:
      options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
      break;
    default:
      std::cout<<"BAUDRATE ERROR"<<std::endl;
      break;
  }
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(uart0_filestream, TCIFLUSH);
  tcsetattr(uart0_filestream, TCSANOW, &options);
  return true;
}

void UART::readBuffer(BufferOperation &buffer, int timeOut){
  time_t starting_time;
  starting_time = time(NULL);
  while (ros::ok()){
    if (uart0_filestream != -1){
      char rx_buffer[256];
      int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
      if (rx_length < 0){
        if((time(NULL)-starting_time)*1000.0 > timeOut)break;
      }
      else if (rx_length == 0){
        if(time(NULL)-starting_time > timeOut)break;
      }
      else {
        buffer.data.insert(buffer.data.end(), std::begin(rx_buffer), std::end(rx_buffer));
      }
    }
  }
}

void UART::writeBuffer(unsigned char *d, uint8_t size){
  write(uart0_filestream, d, size);
}

void UART::closeDevice(){
  close(1);
}
