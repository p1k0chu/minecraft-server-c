#include <stdint.h>

typedef enum S2CStatusPacket { STATUS_RESPONSE, PONG_RESPONSE } S2CStatusPacket;

typedef struct StatusResponse {
    const char *version_name;
    int         version_protocol;
    uint32_t    max_players;
    const char *motd;
} StatusResponse;

/**
 * returns the amount of bytes written
 */
uint32_t write_status_response(char *dst, StatusResponse value);

/**
 * returns the amount of bytes written
 */
uint32_t write_pong_response(char *dst, long timestamp);

