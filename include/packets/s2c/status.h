#pragma once

#include "buffer.h"

#include <stdint.h>

typedef enum S2CStatusPacket { STATUS_RESPONSE, PONG_RESPONSE } S2CStatusPacket;

typedef struct StatusResponse {
    const char *version_name;
    int         version_protocol;
    uint32_t    max_players;
    const char *motd;
} StatusResponse;

bool write_status_response(BufferWriter *dst, const StatusResponse *value);
bool write_pong_response(BufferWriter *dst, long timestamp);

