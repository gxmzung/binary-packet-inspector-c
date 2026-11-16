#include <string.h>
#include "packet_validator.h"
#include "hex_utils.h"
#include "crc16.h"

int validate_sync_word(const BinaryPacket *packet) {
    return packet != NULL && strcmp(packet->sync, "AA55") == 0;
}

int validate_payload_length(const BinaryPacket *packet) {
    if (packet == NULL) return 0;
    int byte_length = hex_payload_byte_length(packet->payload);
    return byte_length == packet->length;
}

int validate_packet_crc(const BinaryPacket *packet) {
    if (packet == NULL) return 0;
    unsigned int calculated = calculate_crc16_hex_payload(packet->payload);
    return calculated == packet->crc;
}

int validate_packet_crc_decoded(const BinaryPacket *packet) {
    if (packet == NULL || !is_valid_hex_string(packet->payload) ||
        !validate_payload_length(packet)) return 0;
    unsigned int calculated = 0;
    return calculate_crc16_decoded_payload(packet->payload, &calculated) &&
           calculated == packet->crc;
}

int count_packet_errors(const BinaryPacket *packet) {
    if (packet == NULL) return 4;
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
