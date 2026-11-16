#ifndef PACKET_VALIDATOR_H
#define PACKET_VALIDATOR_H

#include "packet_reader.h"

int validate_sync_word(const BinaryPacket *packet);
int validate_payload_length(const BinaryPacket *packet);
/* Original default: CRC over ASCII hex representation. */
int validate_packet_crc(const BinaryPacket *packet);
/* Opt-in: CRC-16/MODBUS over decoded bytes, not original wire semantics. */
int validate_packet_crc_decoded(const BinaryPacket *packet);
int count_packet_errors(const BinaryPacket *packet);

#endif
