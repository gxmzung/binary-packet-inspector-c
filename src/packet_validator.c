#include <string.h>
#include "packet_validator.h"
#include "hex_utils.h"
#include "crc16.h"

int validate_sync_word(const BinaryPacket *packet) {
    return strcmp(packet->sync, "AA55") == 0;
}

int validate_payload_length(const BinaryPacket *packet) {
    int byte_length = hex_payload_byte_length(packet->payload);
    return byte_length == packet->length;
}

int validate_packet_crc(const BinaryPacket *packet) {
    unsigned int calculated = calculate_crc16_hex_payload(packet->payload);
    return calculated == packet->crc;
}

int count_packet_errors(const BinaryPacket *packet) {
    int errors = 0;

    if (!validate_sync_word(packet)) {
        errors++;
    }

    if (!is_valid_hex_string(packet->payload)) {
        errors++;
    }

    if (!validate_payload_length(packet)) {
        errors++;
    }

    if (!validate_packet_crc(packet)) {
        errors++;
    }

    return errors;
}