#pragma once

#include <stddef.h>
#include "buffer.h"
#include <stdint.h>

#define CONTINUATION_BIT 0b10000000

void send_var_int(int sockfd, int value);
int  recv_var_int(int sockfd);

bool read_var_int(int *dst, BufferReader *src);
bool write_var_int(BufferWriter *dst, int value);
