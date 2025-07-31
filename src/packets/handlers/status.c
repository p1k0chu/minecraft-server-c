#include "packets/handlers/status.h"

#include "packets/c2s/status.h"
#include "packets/s2c/status.h"
#include "utils/protocol_utils.h"
#include "var_int.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>

void handle_status_packet(PlayerConnection *const conn,
                          const char *const       packet_bytes,
                          const size_t            n_bytes,
                          const C2SStatusPacket   id) {
    switch (id) {
    case STATUS_REQUEST:
        handle_status_request(conn, packet_bytes, n_bytes);
        break;
    case PING_REQUEST:
        handle_ping_request(conn, packet_bytes, n_bytes);
        break;
    }
}

void handle_status_request(PlayerConnection *const conn, const char *const, const size_t) {
    StatusResponse resp = {.version_name     = "1.21.8",
                           .version_protocol = 772,
                           .max_players      = 20,
                           .motd             = "Hello from c!"};
    char           buffer[6969];
    const uint32_t length = write_status_response(buffer, resp);

    send_var_int(conn->socket, length);
    send(conn->socket, buffer, length, 0);
}

void handle_ping_request(PlayerConnection *const conn,
                         const char *const       packet_bytes,
                         const size_t) {
    uint32_t length;

    long timestamp;
    read_ping_request(&timestamp, packet_bytes);

    // VarInt + long
    char buffer[5 + sizeof(long)];
    length = write_pong_response(buffer, timestamp);

    send_var_int(conn->socket, length);
    send(conn->socket, buffer, length, 0);
}
