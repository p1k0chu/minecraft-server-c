#include "var_int.h"

#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

bool read_var_int(int *const dst, BufferReader *const src) {
    *dst = 0;

    char    byte;
    uint8_t shift = 0;

    do {
        if (src->remaining < 1) return false;
        byte = *(src->ptr);
        buffer_reader_increment(src, 1);

        *dst = *dst | ((byte & 127) << shift);
        shift += 7;

        if (shift > 35) {
            return false;
        }
    } while ((byte & CONTINUATION_BIT) != 0);

    return true;
}

bool write_var_int(BufferWriter *const dst, int value) {
    // doing this once is faster than 5 times.
    if (!buffer_writer_ensure_can_write(dst, 5)) {
        return false;
    }

    while ((value & CONTINUATION_BIT) != 0) {
        *(dst->ptr++) = (value & 127) | 128;

        value = (unsigned int)value >> 7;
    }
    *(dst->ptr++) = value & 127;

    return true;
}

void send_var_int(int sockfd, int value) {
    BufferWriter writer = new_buffer_writer(5);

    if (write_var_int(&writer, value)) {
        send(sockfd, writer.start, writer.ptr - writer.start, 0);
    }
}

int recv_var_int(int sockfd) {
    int     result = 0;
    uint8_t shift  = 0;
    char    byte;

    do {
        recv(sockfd, &byte, 1, 0);
        result = result | ((byte & 127) << shift);
        shift += 7;

        if (shift > 35) {
            fprintf(stderr, "VarInt is too long");
            exit(1);
        }
    } while ((byte & CONTINUATION_BIT) != 0);

    return result;
}

