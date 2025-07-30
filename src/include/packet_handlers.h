#pragma once

#include <stddef.h>

void handle_status_request(int sockfd, const char *packet_bytes, size_t n_bytes);
void handle_ping_request(int sockfd, const char *packet_bytes, size_t n_bytes);

