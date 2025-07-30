#include "packet_handlers.h"

#include "packet_definition.h"
#include "packet_ids.h"
#include "server.h"
#include "utils/protocol_utils.h"
#include "utils/utils.h"
#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void handle_handshake(PlayerConnection *const conn,
                      const char *const       packet_bytes,
                      const size_t            n_bytes) {
    UNUSED(n_bytes);

    uint            tmp;
    HandshakePacket packet = read_handshake_packet(packet_bytes, &tmp);

    switch (packet.intent) {
    case STATUS_INTENT:
        conn->stage = STATUS;
        break;
    case LOGIN_INTENT:
        conn->stage = LOGIN;
        break;
    case TRANSFER_INTENT:
        fprintf(stderr, "Transfer not implemented\n");
        exit(1);
    default:
        fprintf(stderr, "invalid handshake intent: %d\n", packet.intent);
        exit(1);
    }

    free_HandshakePacket(packet);
}

void handle_status_request(PlayerConnection *const conn,
                           const char *const       packet_bytes,
                           const size_t            n_bytes) {
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

    send_var_int(conn->socket, length);
    send(conn->socket, buffer, length, 0);
}

void handle_ping_request(PlayerConnection *const conn,
                         const char *const       packet_bytes,
                         const size_t            n_bytes) {
    UNUSED(n_bytes);

    uint length;

    long timestamp = read_ping_request(packet_bytes, &length);

    // VarInt + long
    char buffer[5 + sizeof(long)];
    write_var_int(buffer, PONG_RESPONSE, &length);
    *(long *)(buffer + length) = timestamp;
    length += sizeof(long);

    send_var_int(conn->socket, length);
    send(conn->socket, buffer, length, 0);
}

