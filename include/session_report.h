#ifndef SESSION_REPORT_H
#define SESSION_REPORT_H

#include "packet_reader.h"

typedef struct {
    int total_packets;
    int valid_packets;
    int malformed_packets;
    int critical_packets;
    int crc_errors;
} PacketSessionReport;

void init_packet_session(PacketSessionReport *report);
void update_packet_session(PacketSessionReport *report, const BinaryPacket *packet, int error_count);
void print_packet_session_report(const PacketSessionReport *report);

#endif