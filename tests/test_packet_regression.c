#include "packet_reader.h"
#include "packet_validator.h"
#include "crc16.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    /* Canonical MODBUS check vector over bytes 0x31..0x39. */
    unsigned int byte_crc = 0;
    assert(calculate_crc16_decoded_payload("313233343536373839", &byte_crc));
    assert(byte_crc == 0x4B37u);

    /* Existing default hashes the ASCII hex characters, and must remain so. */
    assert(calculate_crc16_hex_payload("123456789") == 0x4B37u);
    unsigned int legacy_crc = calculate_crc16_hex_payload("120A33FF");
    assert(calculate_crc16_decoded_payload("120A33FF", &byte_crc));
    assert(legacy_crc != byte_crc);

    char record[128];
    snprintf(record, sizeof record, "AA55,01,4,120A33FF,%04X", legacy_crc);
    BinaryPacket p = {0};
    assert(parse_packet_record(record, &p));
    assert(p.type == 1 && p.length == 4);
    assert(validate_sync_word(&p));
    assert(validate_payload_length(&p));
    assert(validate_packet_crc(&p));
    assert(!validate_packet_crc_decoded(&p));
    assert(count_packet_errors(&p) == 0);

    snprintf(record, sizeof record, "AA55,01,4,120A33FF,%04X", byte_crc);
    assert(parse_packet_record(record, &p));
    assert(validate_packet_crc_decoded(&p));
    assert(!validate_packet_crc(&p));

    unsigned int unchanged = 0xABCDu;
    assert(!calculate_crc16_decoded_payload("123", &unchanged));
    assert(unchanged == 0xABCDu);
    assert(!calculate_crc16_decoded_payload("12XZ", &unchanged));
    assert(!calculate_crc16_decoded_payload(NULL, &unchanged));
    assert(!calculate_crc16_decoded_payload("AB", NULL));

    p.type = 7;
    assert(!parse_packet_record(NULL, &p));
    assert(!parse_packet_record("AA55,NOTHEX,4,120A33FF,0000", &p));
    assert(p.type == 7); /* failed parse must not overwrite output */
    assert(!validate_packet_crc_decoded(NULL));
    assert(count_packet_errors(NULL) == 4);

    puts("packet CRC compatibility and format regression: passed");
    return 0;
}
