#pragma once

#include <stdint.h>

/**
 * returns amount of bytes written
 */
uint32_t write_prefixed_bytes(char *dst, const char *src, int n);

/**
 * reads length prefixed bytes from src into a newly allocated array
 * and puts a pointer to that array at dst.
 * returns amount of bytes read from src.
 */
uint32_t read_prefixed_bytes(char **dst, const char *src);

/**
 * same as read_prefixed_bytes but adds \0 at the end
 * (returned length not affected)
 */
uint32_t read_prefixed_string(char **dst, const char *src);

void send_var_int(int sockfd, int value);
int  recv_var_int(int sockfd);

