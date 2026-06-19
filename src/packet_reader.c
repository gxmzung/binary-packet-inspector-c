#include <stdio.h>
#include <string.h>
#include "packet_reader.h"

int parse_packet_record(const char *line, BinaryPacket *packet) {
    if (line == NULL || packet == NULL) {
        return 0;
    }

    int parsed = sscanf(
        line,
        "%7[^,],%x,%d,%127[^,],%x",
        packet->sync,
        &packet->type,
        &packet->length,
        packet->payload,
        &packet->crc
    );

    return parsed == 5;
}

void print_packet_record(const BinaryPacket *packet) {
    if (packet == NULL) {
        return;
    }

    printf(
        "SYNC=%s TYPE=0x%02X LENGTH=%d PAYLOAD=%s CRC=0x%04X\n",
        packet->sync,
        packet->type,
        packet->length,
        packet->payload,
        packet->crc
    );
}