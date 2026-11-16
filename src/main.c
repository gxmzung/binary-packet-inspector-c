#include <stdio.h>
#include <string.h>
#include "packet_reader.h"
#include "packet_validator.h"
#include "packet_classifier.h"
#include "session_report.h"

int main(void) {
    FILE *file = fopen("data/sample_packets.txt", "r");
    if (file == NULL) {
        printf("Failed to open packet data file.\n");
        return 1;
    }

    /* Max valid record with 128-byte payload fits within this line buffer. */
    char line[320];
    BinaryPacket packet;
    PacketSessionReport report;
    init_packet_session(&report);

    while (fgets(line, sizeof line, file)) {
        if (strchr(line, '\n') == NULL && !feof(file)) {
            /* Consume an overlong record rather than processing its fragments. */
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) {}
            fprintf(stderr, "Skipping overlong packet record\n");
            continue;
        }
        if (parse_packet_record(line, &packet)) {
            int errors = count_packet_errors(&packet);
            print_packet_record(&packet);
            printf("Packet class: %s\n", classify_packet_type(packet.type));
            if (errors > 0)
                printf("Warning: packet has %d validation error(s).\n", errors);
            update_packet_session(&report, &packet, errors);
            printf("\n");
        }
    }
    fclose(file);
    print_packet_session_report(&report);
    return 0;
}
