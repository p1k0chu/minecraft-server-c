#pragma once

#include <stdint.h>

#define CONTINUATION_BIT 0b10000000

/**
 * reads one VarInt from src into dst, returns the amoung of bytes read
 */
uint32_t read_var_int(int *dst, const char *src);

/**
 * returns the amount of bytes written
 */
uint32_t write_var_int(char *dst, int value);
