#include "packet_classifier.h"

const char *classify_packet_type(int type) {
    switch (type) {
        case 0x01:
            return "HEALTH_STATUS";
        case 0x02:
            return "MISSION_DATA";
        case 0x03:
            return "SENSOR_FRAME";
        case 0x04:
            return "WARNING_EVENT";
        case 0x05:
            return "CONTROL_ACK";
        default:
            return "UNKNOWN_PACKET";
    }
}

int is_critical_packet_type(int type) {
    return type == 0x04;
}