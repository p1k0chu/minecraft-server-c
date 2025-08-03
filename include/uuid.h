#pragma once

#include <uuid/uuid.h>

void uuid_generate_md5_no_ns(uuid_t out, const char *name, size_t len);
