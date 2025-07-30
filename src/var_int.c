#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint32_t read_var_int(int *const dst, const char *const src) {
    *dst = 0;

    char     byte;
    uint32_t index = 0;

    do {
        byte = src[index];
        *dst = *dst | ((byte & 127) << (index++ * 7));

        if (index > 5) {
            fprintf(stderr, "VarInt is too long");
            exit(1);
        }
    } while ((byte & CONTINUATION_BIT) != 0);

    return index;
}

uint32_t write_var_int(char *const dst, int value) {
    uint32_t index = 0;

    while ((value & CONTINUATION_BIT) != 0) {
        dst[index++] = (value & 127) | 128;

        value = (unsigned int)value >> 7;
    }
    dst[index++] = value & 127;

    return index;
}
