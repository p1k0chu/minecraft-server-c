#include "packets/handlers/status.h"

#include "buffer.h"
#include "packets/c2s/status.h"
#include "packets/s2c/status.h"
#include "var_int.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>

void handle_status_packet(PlayerConnection *conn, BufferReader *packet_reader, C2SStatusPacket id) {
    switch (id) {
    case STATUS_REQUEST:
        handle_status_request(conn, packet_reader);
        break;
    case PING_REQUEST:
        handle_ping_request(conn, packet_reader);
        break;
    }
}

void handle_status_request(PlayerConnection *conn, BufferReader *) {
    const StatusResponse resp   = {.version_name     = "1.21.8",
                                   .version_protocol = 772,
                                   .max_players      = 20,
                                   .motd             = "Hello from c!"};
    BufferWriter         writer = new_buffer_writer(0);
    if (write_status_response(&writer, &resp)) {
        const uint32_t length = writer.ptr - writer.start;

        send_var_int(conn->socket, length);
        send(conn->socket, writer.start, length, 0);
    }

    free(writer.start);
}

void handle_ping_request(PlayerConnection *conn, BufferReader *packet_reader) {
    long timestamp;
    if (!read_ping_request(&timestamp, packet_reader)) return;

    BufferWriter writer = new_buffer_writer(sizeof(long) + 5);
    if (write_pong_response(&writer, timestamp)) {
        const uint32_t length = writer.ptr - writer.start;
        send_var_int(conn->socket, length);
        send(conn->socket, writer.start, length, 0);
    }

    free(writer.start);
}
