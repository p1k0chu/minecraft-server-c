#include "utils.h"

#include "uuid.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OFFLINE_PLAYER_FMT "OfflinePlayer:%.*s"

void uuid_for_offline_player(uuid_t out, const char *const username) {
    int old_len = (int)strlen(username);
    int new_len = snprintf(NULL, 0, OFFLINE_PLAYER_FMT, old_len, username);

    char *new_str = malloc(new_len + 1);
    if (!new_str) ERROR("malloc");

    sprintf(new_str, OFFLINE_PLAYER_FMT, old_len, username);

    uuid_generate_md5_no_ns(out, new_str, new_len);

    free(new_str);
}

