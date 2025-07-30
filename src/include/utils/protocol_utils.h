#pragma once

#include <sys/types.h>

void write_prefixed_bytes(char *dst, const char *src, int n, uint *out_written_bytes);
void send_var_int(int sockfd, int value);

