#pragma once

#include <sys/types.h>

typedef struct StatusResponse {
    const char *const version_name;
    const int   version_protocol;
    const uint  max_players;
    const char *const motd;
} StatusResponse;

void write_status_response(char *dst, StatusResponse value, uint *out_written_bytes);

long read_ping_request(const char *src, uint *out_consumed_bytes);
void write_pong_response(char *dst, long timestamp, uint *out_written_bytes);

