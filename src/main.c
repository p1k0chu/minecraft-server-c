#include "packet_handlers.h"
#include "packet_ids.h"
#include "server.h"
#include "utils/protocol_utils.h"
#include "utils/utils.h"
#include "var_int.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 25565

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    socklen_t          sock_length = sizeof(server_addr);

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port        = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sock_length) < 0) {
        error("error on binding");
    }

    if (listen(sockfd, 5) == -1) {
        error("error on listening");
    };

    while (1) {
        int socket = accept(sockfd, (struct sockaddr *)&server_addr, &sock_length);
        if (socket < 0) {
            error("error while accepting");
        }

        int pid = fork();
        if (pid == -1) {
            error("error forking");
        } else if (pid != 0) { // old proccess
            continue;
        }

        PlayerConnection conn = {.stage = HANDSHAKING, .socket = socket};
        int              packet_id;
        int              packet_length;
        char            *buffer        = NULL;
        int              buffer_length = 0;
        uint             buffer_index  = 0;

#define BUFFER_READER (buffer + buffer_index)
#define BUFFER_LENGTH (buffer_length - buffer_index)

        while (1) {
            packet_length = recv_var_int(socket);

            if (buffer_length < packet_length || buffer == NULL) {
                if (buffer != NULL) free(buffer);

                buffer_length = packet_length;
                buffer        = malloc(buffer_length);

                if (buffer == NULL) error("Error allocating buffer for a packet");
            }

            recv(socket, buffer, packet_length, 0);

            packet_id = read_var_int(buffer, &buffer_index);

            switch (conn.stage) {
            case HANDSHAKING:
                switch ((C2SHandshakingPacket)packet_id) {
                case HANDSHAKE:
                    handle_handshake(conn, BUFFER_READER, BUFFER_LENGTH);
                    break;
                }
                break;
            case STATUS:
            case LOGIN:
            case CONFIGURATION:
            case PLAY:
                fprintf(stderr, "Protocol stage %d is not implemented!\n", conn.stage);
                close(socket);
                exit(1);
            }

#undef BUFFER_READER
#undef BUFFER_LENGTH
        }

        // close(socket);
        // exit(0);
    }

    return 0;
}
