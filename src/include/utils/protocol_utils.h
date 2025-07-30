#pragma once

#include <sys/types.h>

/**
 * returns amoung of bytes written
 */
uint write_prefixed_bytes(char *dst, const char *src, int n);

/**
 * reads length prefixed bytes from src into a newly allocated array
 * and puts a pointer to that array at dst.
 * returns amount of bytes read from src.
 */
uint read_prefixed_bytes(char **dst, const char *src);

void send_var_int(int sockfd, int value);
int  recv_var_int(int sockfd);

