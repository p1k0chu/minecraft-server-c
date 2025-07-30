#pragma once

#include <sys/types.h>

#define CONTINUATION_BIT 0b10000000

/**
 * reads one VarInt from src into dst, returns the amoung of bytes read
 */
uint read_var_int(int *dst, const char *src);

/**
 * returns the amount of bytes written
 */
uint write_var_int(char *dst, int value);
