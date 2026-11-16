#ifndef PACKET_READER_H
#define PACKET_READER_H

/* 128 decoded payload bytes require 256 ASCII hex characters + NUL. */
#define BINARY_PACKET_MAX_BYTES 128
#define BINARY_PACKET_HEX_CAPACITY (2 * BINARY_PACKET_MAX_BYTES + 1)

typedef struct {
    char sync[8];
    int type;
    int length;
    char payload[BINARY_PACKET_HEX_CAPACITY];
    unsigned int crc;
} BinaryPacket;

int parse_packet_record(const char *line, BinaryPacket *packet);
void print_packet_record(const BinaryPacket *packet);

#endif
