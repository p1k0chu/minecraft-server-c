#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int read_var_int(const void *const src, uint *const out_consumed_bytes) {
#define char_src ((const char *const)src)
#define index    (*out_consumed_bytes)

    int  result = 0;
    char byte;
    index = 0;

    do {
        byte   = char_src[index];
        result = result | ((byte & 127) << (index++ * 7));

        if (index > 5) {
            fprintf(stderr, "VarInt is too long");
            exit(1);
        }
    } while ((byte & CONTINUATION_BIT) != 0);

    return result;

#undef char_src
#undef index
}

void write_var_int(void *const dst, int value, uint *const out_written_bytes) {
#define char_dst ((char *const)dst)
#define index    (*out_written_bytes)

    index = 0;
    while ((value & CONTINUATION_BIT) != 0) {
        char_dst[index++] = (value & 127) | 128;

        value = (unsigned int)value >> 7;
    }
    char_dst[index++] = value & 127;

#undef char_dst
#undef index
}
