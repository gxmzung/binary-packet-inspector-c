#ifndef PACKET_READER_H
#define PACKET_READER_H

typedef struct {
    char sync[8];
    int type;
    int length;
    char payload[128];
    unsigned int crc;
} BinaryPacket;

int parse_packet_record(const char *line, BinaryPacket *packet);
void print_packet_record(const BinaryPacket *packet);

#endif