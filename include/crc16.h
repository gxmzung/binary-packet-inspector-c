#ifndef CRC16_H
#define CRC16_H

/* Legacy CRC: processes each ASCII character in the hex representation.
   Preserved for compatibility with the original inspector. */
unsigned int calculate_crc16_hex_payload(const char *hex_payload);

/* Standard CRC-16/MODBUS over bytes decoded from a hex string.
   Returns 1 if valid and stores the CRC in *out_crc; 0 for invalid input.
   This is an opt-in API, not a silent change of the legacy CRC contract. */
int calculate_crc16_decoded_payload(const char *hex_payload, unsigned int *out_crc);

#endif
