#include <stdio.h>
#include <string.h>
#include "packet_reader.h"

/* A checked, comma-separated textual record parser; never changes output on
   malformed records. It does not validate sync or checksum: the validator
   reports those independently, preserving the original diagnostic behavior. */
static int hex_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static int parse_field_number(const char *s, size_t n, unsigned max,
                              int base, unsigned *out) {
    if (n == 0 || out == NULL) return 0;
    unsigned v = 0;
    for (size_t i = 0; i < n; ++i) {
        int digit = base == 16 ? hex_value(s[i]) :
                    (s[i] >= '0' && s[i] <= '9' ? s[i] - '0' : -1);
        if (digit < 0 || digit >= base || v > (max - (unsigned)digit) / (unsigned)base)
            return 0;
        v = v * (unsigned)base + (unsigned)digit;
    }
    *out = v;
    return 1;
}

int parse_packet_record(const char *line, BinaryPacket *packet) {
    if (line == NULL || packet == NULL) return 0;
    /* Upper bound includes 256 hex, small scalar fields and line terminator. */
    size_t n = 0;
    while (n < 320 && line[n] != '\0') ++n;
    if (n == 320) return 0;
    if (n && line[n - 1] == '\n') --n;
    if (n && line[n - 1] == '\r') --n;

    const char *field[5];
    size_t width[5];
    size_t start = 0, part = 0;
    for (size_t i = 0; i <= n; ++i) {
        if (i != n && line[i] != ',') continue;
        if (part == 5) return 0;
        field[part] = line + start;
        width[part++] = i - start;
        start = i + 1;
    }
    if (part != 5 || width[0] == 0 || width[0] >= sizeof packet->sync ||
        width[1] == 0 || width[1] > 2 || width[2] == 0 || width[2] > 3 ||
        width[3] >= sizeof packet->payload || width[4] == 0 || width[4] > 4)
        return 0;

    unsigned type = 0, declared = 0, crc = 0;
    if (!parse_field_number(field[1], width[1], 255, 16, &type) ||
        !parse_field_number(field[2], width[2], BINARY_PACKET_MAX_BYTES, 10, &declared) ||
        !parse_field_number(field[4], width[4], 0xFFFF, 16, &crc))
        return 0;

    BinaryPacket parsed = {0};
    memcpy(parsed.sync, field[0], width[0]);
    parsed.type = (int)type;
    parsed.length = (int)declared;
    memcpy(parsed.payload, field[3], width[3]);
    parsed.crc = crc;
    *packet = parsed;
    return 1;
}

void print_packet_record(const BinaryPacket *packet) {
    if (packet == NULL) return;
    printf("SYNC=%s TYPE=0x%02X LENGTH=%d PAYLOAD=%s CRC=0x%04X\n",
           packet->sync, packet->type, packet->length, packet->payload, packet->crc);
}
