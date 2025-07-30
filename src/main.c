#include "utils/utils.h"

#include <netinet/in.h>
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

        const char message[] = ":3\n";
        send(socket, message, sizeof(message), 0);

        close(socket);
        exit(0);
    }

    return 0;
}
