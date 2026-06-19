#ifndef PACKET_VALIDATOR_H
#define PACKET_VALIDATOR_H

#include "packet_reader.h"

int validate_sync_word(const BinaryPacket *packet);
int validate_payload_length(const BinaryPacket *packet);
int validate_packet_crc(const BinaryPacket *packet);
int count_packet_errors(const BinaryPacket *packet);

#endif