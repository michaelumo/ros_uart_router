
#ifndef BUFFEROPERATION_
  #define BUFFEROPERATION_

class BufferOperation {
private:
  void split(char* d, char* other, int a, int b);
  float byte2float(int, int);
  int sum(char*, int);
public:
  int datalen = 0;
  uint8_t* data[256];
  BufferOperation();
  int sumofPayload(int);
  int getChecksum();
  void clearBuffer();
  int sizeofPayload();
};

#endif
