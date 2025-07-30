#pragma once

#include <stddef.h>
#include "server.h"

void handle_status_request(PlayerConnection conn, const char *packet_bytes, size_t n_bytes);
void handle_ping_request(PlayerConnection conn, const char *packet_bytes, size_t n_bytes);

