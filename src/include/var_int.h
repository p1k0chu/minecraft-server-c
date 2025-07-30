#pragma once

#include <sys/types.h>

#define CONTINUATION_BIT 0b10000000

int read_var_int(const char *src, uint *out_consumed_bytes);

void write_var_int(char *dst, int value, uint *out_written_bytes);
