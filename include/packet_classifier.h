#ifndef PACKET_CLASSIFIER_H
#define PACKET_CLASSIFIER_H

const char *classify_packet_type(int type);
int is_critical_packet_type(int type);

#endif