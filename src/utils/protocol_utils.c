#include "utils/protocol_utils.h"

#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void write_prefixed_bytes(char *const       dst,
                          const char *const src,
                          const int         n,
                          uint             *out_written_bytes) {
    write_var_int(dst, n, out_written_bytes);

    memcpy(dst + *out_written_bytes, src, n);
}

void send_var_int(int sockfd, int value) {
    char buffer[5];
    uint length;
    write_var_int(buffer, value, &length);

    send(sockfd, buffer, length, 0);
}

int recv_var_int(int sockfd) {
    int  result = 0;
    int  shift  = 0;
    char byte;

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

