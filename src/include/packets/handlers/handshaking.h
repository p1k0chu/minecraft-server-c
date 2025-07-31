#pragma once

#include "server.h"

#include <stddef.h>

void handle_handshake(PlayerConnection *conn, const char *packet_bytes, size_t n_bytes);

