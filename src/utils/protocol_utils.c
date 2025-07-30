#include "utils/protocol_utils.h"

#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

uint write_prefixed_bytes(char *const dst, const char *const src, const int n) {
    uint length = write_var_int(dst, n);

    memcpy(dst + length, src, n);
    return length + n;
}

uint read_prefixed_bytes(char **const dst, const char *src) {
    int length;
    uint bytes_read = read_var_int(&length, src);

    char *buffer = malloc(length);
    memcpy(buffer, src + bytes_read, length);
    *dst = buffer;

    return bytes_read + length;
}

void send_var_int(int sockfd, int value) {
    char buffer[5];
    uint length = write_var_int(buffer, value);

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

