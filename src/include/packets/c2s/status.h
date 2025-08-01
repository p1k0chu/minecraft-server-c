#pragma once

#include "buffer.h"
#include <stdint.h>

typedef enum C2SStatusPacket { STATUS_REQUEST, PING_REQUEST } C2SStatusPacket;

///reads one ping request(the timestamp) from src into dst and returns success
bool read_ping_request(long *dst, BufferReader *src);

