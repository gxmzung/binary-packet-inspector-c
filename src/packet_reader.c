#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "packet_reader.h"

int parse_packet_record(const char *line, BinaryPacket *packet) {
    if (line == NULL || packet == NULL) {
        return 0;
    }

    /* %x requires unsigned int*, not int*. Decode to the correct type
       and convert only after the complete record has been parsed. */
    unsigned int packet_type = 0;
    BinaryPacket parsed_packet = {0};
    int parsed = sscanf(
        line,
        "%7[^,],%x,%d,%127[^,],%x",
        parsed_packet.sync,
        &packet_type,
        &parsed_packet.length,
        parsed_packet.payload,
        &parsed_packet.crc
    );
    if (parsed != 5 || packet_type > INT_MAX) {
        return 0;
    }

    parsed_packet.type = (int)packet_type;
    *packet = parsed_packet;
    return 1;
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
