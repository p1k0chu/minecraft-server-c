#pragma once

#include <sys/types.h>

#define CONTINUATION_BIT 0b10000000

int read_var_int(const int sock_fd);

void write_var_int(const int sock_fd, const int value);

