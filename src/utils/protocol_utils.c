#include "utils/protocol_utils.h"

#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

uint32_t write_prefixed_bytes(char *const dst, const char *const src, const int n) {
    uint32_t length = write_var_int(dst, n);

    memcpy(dst + length, src, n);
    return length + n;
}

uint32_t read_prefixed_bytes(char **const dst, const char *src) {
    int      length;
    uint32_t bytes_read = read_var_int(&length, src);

    *dst = malloc(length);
    memcpy(*dst, src + bytes_read, length);

    return bytes_read + length;
}

uint32_t read_prefixed_string(char **const dst, const char *const src) {
    int      length;
    uint32_t bytes_read = read_var_int(&length, src);

    *dst = malloc(length + 1);
    memcpy(*dst, src + bytes_read, length);
    *dst[length] = 0;

    return bytes_read + length;
}

void send_var_int(int sockfd, int value) {
    char     buffer[5];
    uint32_t length = write_var_int(buffer, value);

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

