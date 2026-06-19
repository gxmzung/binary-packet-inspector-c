#include "crc16.h"

unsigned int calculate_crc16_hex_payload(const char *hex_payload) {
    unsigned int crc = 0xFFFF;

    for (int i = 0; hex_payload[i] != '\0'; i++) {
        crc ^= (unsigned char)hex_payload[i];

        for (int bit = 0; bit < 8; bit++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc & 0xFFFF;
}