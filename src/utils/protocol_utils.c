#include "utils/protocol_utils.h"

#include "var_int.h"

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

