#include "buffer.h"
#include "packets/handlers/handshaking.h"
#include "packets/handlers/status.h"
#include "server.h"
#include "utils.h"
#include "var_int.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 25565

static inline bool handle_recv_error(long result) {
    if (result == 0) {
        fprintf(stderr, "socket was closed :<\n");
        return false;
    }

    if (result < 0) {
        ERROR("error reading from socket");
    }

    return true;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    socklen_t          sock_length = sizeof(server_addr);

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sock_length) < 0) {
        ERROR("error on binding");
    }

    if (listen(sockfd, 5) == -1) {
        ERROR("error on listening");
    };

    while (1) {
        int socket = accept(sockfd, (struct sockaddr *)&server_addr, &sock_length);
        if (socket < 0) {
            ERROR("error while accepting");
        }

        int pid = fork();
        if (pid == -1) {
            ERROR("error forking");
        } else if (pid != 0) { // old process
            continue;
        }

        PlayerConnection conn = {.stage = HANDSHAKING, .socket = socket};
        int              packet_id;
        int              packet_length;
        char            *buffer        = NULL;
        int              buffer_length = 0;

        while (1) {
            if (!(handle_recv_error(recv_var_int(&packet_length, socket)))) {
                break;
            }

            if (buffer_length < packet_length || buffer == NULL) {
                buffer_length = packet_length;
                buffer        = realloc(buffer, buffer_length);

                if (buffer == NULL) ERROR("Error allocating buffer for a packet");
            }

            if (!(handle_recv_error(recv(socket, buffer, packet_length, 0)))) {
                break;
            }

            BufferReader reader = {.ptr = buffer, .remaining = packet_length};

            read_var_int(&packet_id, &reader);

            switch (conn.stage) {
            case HANDSHAKING:
                handle_handshaking_packet(&conn, &reader, packet_id);
                break;
            case STATUS:
                handle_status_packet(&conn, &reader, packet_id);
                break;
            case LOGIN:
            case CONFIGURATION:
            case PLAY:
                fprintf(stderr, "Protocol stage %d is not implemented!\n", conn.stage);
                close(socket);
                exit(1);
            }
        }
    }

    return 0;
}
