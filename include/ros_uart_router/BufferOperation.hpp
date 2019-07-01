
#ifndef BUFFEROPERATION_
  #define BUFFEROPERATION_

class BufferOperation {
private:
  union cksmConv{
    uint8_t in8[2];
    uint16_t in16;
  };
  cksmConv cksm;
  void split(std::vector<uint8_t> &d, std::vector<uint8_t> &other, uint8_t a, uint8_t b);
public:
  std::vector<uint8_t> data;
  BufferOperation();
  uint16_t generateChecksum(uint8_t, uint8_t);
  uint16_t getChecksum();
  void clearBuffer();
  uint8_t lengthofPayload();
};

#endif
