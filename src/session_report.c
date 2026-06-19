#include <stdio.h>
#include "session_report.h"
#include "packet_classifier.h"
#include "packet_validator.h"

void init_packet_session(PacketSessionReport *report) {
    report->total_packets = 0;
    report->valid_packets = 0;
    report->malformed_packets = 0;
    report->critical_packets = 0;
    report->crc_errors = 0;
}

void update_packet_session(PacketSessionReport *report, const BinaryPacket *packet, int error_count) {
    report->total_packets++;

    if (error_count == 0) {
        report->valid_packets++;
    } else {
        report->malformed_packets++;
    }

    if (is_critical_packet_type(packet->type)) {
        report->critical_packets++;
    }

    if (!validate_packet_crc(packet)) {
        report->crc_errors++;
    }
}

void print_packet_session_report(const PacketSessionReport *report) {
    printf("\n=== Binary Packet Session Report ===\n");
    printf("Total packets: %d\n", report->total_packets);
    printf("Valid packets: %d\n", report->valid_packets);
    printf("Malformed packets: %d\n", report->malformed_packets);
    printf("Critical packets: %d\n", report->critical_packets);
    printf("CRC errors: %d\n", report->crc_errors);

    if (report->malformed_packets == 0) {
        printf("Session status: CLEAN\n");
    } else {
        printf("Session status: NEEDS INSPECTION\n");
    }
}