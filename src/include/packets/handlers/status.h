#pragma once

#include "buffer.h"
#include "packets/c2s/status.h"
#include "server.h"

#include <stddef.h>

void handle_status_packet(PlayerConnection *conn, BufferReader *packet_reader, C2SStatusPacket id);

void handle_status_request(PlayerConnection *conn, BufferReader *packet_reader);
void handle_ping_request(PlayerConnection *conn, BufferReader *packet_reader);

