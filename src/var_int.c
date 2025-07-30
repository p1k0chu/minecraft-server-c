#include "var_int.h"

#include "sys/socket.h"
#include "utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int read_var_int(const int sock_fd) {
    int           result = 0;
    unsigned char shift  = 0;

    char byte;
    do {
        if (recv(sock_fd, &byte, 1, 0) < 0) {
            error("error reading VarInt: recv");
        };
        result = result | ((byte & 127) << shift);
        shift += 7;

        if (shift > 35) {
            fprintf(stderr, "VarInt is too long");
            exit(1);
        }
    } while ((byte & CONTINUATION_BIT) != 0);

    return result;
}

void write_var_int(const int sock_fd, int value) {
    char byte;

    while ((value & CONTINUATION_BIT) != 0) {
        byte = (value & 127) | 128;
        write(sock_fd, &byte, 1);

        value = (unsigned int)value >> 7;
    }
    byte = value & 127;
    write(sock_fd, &byte, 1);
}
