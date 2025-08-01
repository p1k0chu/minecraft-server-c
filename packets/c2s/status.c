#include "packets/c2s/status.h"

#include "buffer.h"

#include <stdint.h>

bool read_ping_request(long *const dst, BufferReader *const src) {
    if (src->remaining < sizeof(long)) return false;

    *dst = *(const long *)(src->ptr);
    buffer_reader_increment(src, sizeof(long));

    return true;
}

