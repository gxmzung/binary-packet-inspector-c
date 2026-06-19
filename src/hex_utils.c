#include <ctype.h>
#include <string.h>
#include "hex_utils.h"

int is_valid_hex_string(const char *value) {
    if (value == 0 || strlen(value) == 0) {
        return 0;
    }

    for (int i = 0; value[i] != '\0'; i++) {
        if (!isxdigit((unsigned char)value[i])) {
            return 0;
        }
    }

    return 1;
}

int hex_payload_byte_length(const char *payload) {
    if (payload == 0) {
        return 0;
    }

    int hex_len = (int)strlen(payload);

    if (hex_len % 2 != 0) {
        return -1;
    }

    return hex_len / 2;
}