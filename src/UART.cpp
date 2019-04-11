#include <cstdint>
#include <iostream>
#include <unistd.h>			//
#include <fcntl.h>			//
#include <termios.h>		//
#include <ctime>
#include "ros_uart_router/BufferOperation.hpp"
#include "ros_uart_router/UART.hpp"

UART::UART(const char* dev, int bd){
  device = dev;
  baudrate = bd;
}

void UART::setup(){
  uart0_filestream = open(device, O_RDWR | O_NOCTTY | O_NDELAY);          //Open in non blocking r$
  if (uart0_filestream == -1)
  {
          //ERROR - CAN'T OPEN SERIAL PORT
          std::cout << "Error - Unable to open UART.  Ensure it is not in use by another application\n" << std::endl;
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
}

void UART::readBuffer(BufferOperation &buffer, int timeOut){// timout in milliseconds
  time_t starting_time;
  starting_time = time(NULL);
  while (1){
    if (uart0_filestream != -1){
      // Read up to 255 characters from the port if they are there
      char rx_buffer[256];
      int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
      if (rx_length < 0){
        if((time(NULL)-starting_time)*1000 > timeOut)break;
        //An error occured (will occur if there are no bytes)
      }
      else if (rx_length == 0){
        if(time(NULL)-starting_time > timeOut)break;
      }
      else {
        //Bytes received
        for(int i = 0; i < rx_length; i++){
          buffer.data[i+buffer.datalen] = rx_buffer[i];
        }
        buffer.data[buffer.datalen+rx_length] = '\0';
        buffer.datalen += rx_length;
      }
    }
  }
}

void UART::writeBuffer(uint8_t* d, uint8_t size){
  write(uart0_filestream, d, size);
}
