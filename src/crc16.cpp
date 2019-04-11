// This code generates CRC code by CRC16/ARC algorithm

#include <stdio.h>
#include <cstdint>
#include "nymph/crc16.hpp"

uint16_t crc16_table[256];

void crc16Init(void) {
    for (uint16_t i = 0; i < 256; i++) {
        uint16_t c = i;
        for (int j = 0; j < 8; j++) {
            c = (c & 1) ? (0xA001 ^ (c >> 1)) : (c >> 1);
        }
        crc16_table[i] = c;
    }
}

uint16_t crc16(uint8_t *buf, size_t len) {
    uint16_t c = 0;
    for (size_t i = 0; i < len; i++) {
        c = crc16_table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    }
    return c;
}
