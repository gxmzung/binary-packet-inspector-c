#include "packet_reader.h"
#include "packet_validator.h"
#include "crc16.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check_length(unsigned count) {
    char payload[BINARY_PACKET_HEX_CAPACITY] = {0};
    for (unsigned i = 0; i < count; ++i) {
        payload[2*i] = "0123456789ABCDEF"[i & 15u];
        payload[2*i+1] = "0123456789ABCDEF"[(i+7u) & 15u];
    }
    unsigned crc = 0;
    assert(calculate_crc16_decoded_payload(payload, &crc));
    char line[320] = {0};
    int written = snprintf(line, sizeof line, "AA55,01,%u,%s,%04X", count, payload, crc);
    assert(written > 0 && (size_t)written < sizeof line);
    BinaryPacket packet = {0};
    assert(parse_packet_record(line, &packet));
    assert(packet.length == (int)count);
    assert(validate_payload_length(&packet));
    assert(validate_packet_crc_decoded(&packet));
    assert(strlen(packet.payload) == 2u * count);
    packet.crc ^= 1u;
    assert(!validate_packet_crc_decoded(&packet));
}

int main(void) {
    check_length(0);
    check_length(1);
    check_length(63);
    check_length(64);
    check_length(127);
    check_length(128);

    BinaryPacket p = {.type = 77};
    assert(!parse_packet_record("AA55,01,129,00,0000", &p));
    assert(!parse_packet_record("AA55,01,999,00,0000", &p));
    assert(!parse_packet_record("AA55,GG,1,00,0000", &p));
    assert(!parse_packet_record("AA55,01,1,00,ZZZZ", &p));
    assert(!parse_packet_record("AA55,01,1,00,0000,more", &p));
    assert(!parse_packet_record("AA55,01,1,00,0000trailing", &p));
    assert(!parse_packet_record("AA55,01,-1,00,0000", &p));
    assert(p.type == 77);

    /* Preserve legacy checksum over ASCII hex representation as default. */
    const unsigned legacy = calculate_crc16_hex_payload("ABCD");
    char line[100];
    snprintf(line, sizeof line, "BB66,02,2,ABCD,%04X", legacy);
    assert(parse_packet_record(line, &p));
    assert(!validate_sync_word(&p));
    assert(validate_packet_crc(&p));
    assert(count_packet_errors(&p) == 1);

    /* Explicit CRLF record; support zero-byte records without false errors. */
    assert(parse_packet_record("AA55,01,0,,FFFF\r\n", &p));
    assert(validate_packet_crc_decoded(&p));
    assert(count_packet_errors(&p) == 0);
    puts("payload boundary checks: passed");
    return 0;
}
