#include "packets/c2s/status.h"

#include <stdint.h>

uint32_t read_ping_request(long *dst, const char *src) {
    *dst = *((const long *)src);
    return sizeof(long);
}
