#include "packet_handlers.h"

#include "packet_definition.h"
#include "packet_ids.h"
#include "utils/protocol_utils.h"
#include "utils/utils.h"
#include "var_int.h"

#include <sys/socket.h>

void handle_status_request(int sockfd, const char *packet_bytes, size_t n_bytes) {
    UNUSED(packet_bytes);
    UNUSED(n_bytes);

    StatusResponse resp = {.version_name     = "1.21.8",
                           .version_protocol = 772,
                           .max_players      = 20,
                           .motd             = "Hello from c!"};
    char           buffer[6969];
    uint           length;
    uint           tmp;
    write_var_int(buffer, STATUS_RESPONSE, &length);
    write_status_response(buffer + length, resp, &tmp);
    length += tmp;

    send_var_int(sockfd, length);
    send(sockfd, buffer, length, 0);
}

void handle_ping_request(int sockfd, const char *packet_bytes, size_t n_bytes) {
    UNUSED(sockfd);
    UNUSED(packet_bytes);
    UNUSED(n_bytes);
    // TODO: handle_ping_request
}

