#pragma once

#include <stddef.h>
#include "buffer.h"
#include <stdint.h>

#define CONTINUATION_BIT 0b10000000

/// returns success/error code (>0 success, -1 == socket error, -2 == buffer error)
int send_var_int(int sockfd, int value);

/// reads one VarInt from socket into dst.
/// returns success/error code (>0 success, 0 == socket closed, -1 == error)
int  recv_var_int(int *dst, int sockfd);

bool read_var_int(int *dst, BufferReader *src);
bool write_var_int(BufferWriter *dst, int value);
