#include "utils/protocol_utils.h"

#include "var_int.h"
#include <string.h>

void write_prefixed_bytes(char *const       dst,
                          const char *const src,
                          const int         n,
                          uint             *out_written_bytes) {
    write_var_int(dst, n, out_written_bytes);

    memcpy(dst + *out_written_bytes, src, n);
}
