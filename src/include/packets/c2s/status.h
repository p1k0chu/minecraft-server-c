#pragma once

#include <stdint.h>

typedef enum C2SStatusPacket { STATUS_REQUEST, PING_REQUEST } C2SStatusPacket;

/**
 * reads one ping request from src into dst and returns amount of bytes read
 *
 * the whole ping request is just one long (timestamp)
 */
uint32_t read_ping_request(long *dst, const char *src);

