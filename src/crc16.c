#include "crc16.h"
#include <stddef.h>

static int hex_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static unsigned int crc16_update(unsigned int crc, unsigned char byte) {
    crc ^= byte;
    for (int bit = 0; bit < 8; ++bit) {
        crc = (crc & 1u) ? (crc >> 1) ^ 0xA001u : crc >> 1;
    }
    return crc & 0xFFFFu;
}

unsigned int calculate_crc16_hex_payload(const char *hex_payload) {
    unsigned int crc = 0xFFFFu;
    if (hex_payload == NULL) return crc; /* legacy-compatible empty CRC */
    for (size_t i = 0; hex_payload[i] != '\0'; ++i) {
        crc = crc16_update(crc, (unsigned char)hex_payload[i]);
    }
    return crc;
}

int calculate_crc16_decoded_payload(const char *hex_payload, unsigned int *out_crc) {
    if (hex_payload == NULL || out_crc == NULL) return 0;
    unsigned int crc = 0xFFFFu;
    size_t i = 0;
    for (; hex_payload[i] != '\0';) {
        int hi = hex_digit(hex_payload[i++]);
        if (hi < 0 || hex_payload[i] == '\0') return 0;
        int lo = hex_digit(hex_payload[i++]);
        if (lo < 0) return 0;
        crc = crc16_update(crc, (unsigned char)((hi << 4) | lo));
    }
    *out_crc = crc;
    return 1;
}
