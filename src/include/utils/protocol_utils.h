#pragma once

#include <sys/types.h>

void  write_prefixed_bytes(char *dst, const char *src, int n, uint *out_written_bytes);
void *read_prefixed_bytes(const char *src, uint *out_consumed_bytes);
void  send_var_int(int sockfd, int value);
int   recv_var_int(int sockfd);

