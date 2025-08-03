/*
 * Original code from util-linux project.
 * Modified by p1k0chu:
 *   - adapted `uuid_generate_md5` function to generate a uuid without a namespace
 *   - no other meaningful changes except reformatting
 *
 * Original license:
 *
 * Copyright (C) 1996, 1997, 1998, 1999 Theodore Ts'o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ALL OF
 * WHICH ARE HEREBY DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF NOT ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "uuid.h"

#include "md5.h"

#include <string.h>

struct uuid {
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint16_t clock_seq;
    uint8_t  node[6];
};

void uuid_pack(const struct uuid *uu, uuid_t ptr);
void uuid_unpack(const uuid_t in, struct uuid *uu);

void uuid_generate_md5_no_ns(uuid_t out, const char *name, size_t len) {
    UL_MD5_CTX  ctx;
    char        hash[UL_MD5LENGTH];
    uuid_t      buf;
    struct uuid uu;

    ul_MD5Init(&ctx);
    // ul_MD5Update(&ctx, ns, sizeof(uuid_t));
    ul_MD5Update(&ctx, (const unsigned char *)name, len);
    ul_MD5Final((unsigned char *)hash, &ctx);

    // assert(sizeof(buf) <= sizeof(hash));

    memcpy(buf, hash, sizeof(buf));
    uuid_unpack(buf, &uu);

    uu.clock_seq           = (uu.clock_seq & 0x3FFF) | 0x8000;
    uu.time_hi_and_version = (uu.time_hi_and_version & 0x0FFF) | 0x3000;
    uuid_pack(&uu, out);
}

void uuid_pack(const struct uuid *uu, uuid_t ptr) {
    uint32_t       tmp;
    unsigned char *out = ptr;

    tmp    = uu->time_low;
    out[3] = (unsigned char)tmp;
    tmp >>= 8;
    out[2] = (unsigned char)tmp;
    tmp >>= 8;
    out[1] = (unsigned char)tmp;
    tmp >>= 8;
    out[0] = (unsigned char)tmp;

    tmp    = uu->time_mid;
    out[5] = (unsigned char)tmp;
    tmp >>= 8;
    out[4] = (unsigned char)tmp;

    tmp    = uu->time_hi_and_version;
    out[7] = (unsigned char)tmp;
    tmp >>= 8;
    out[6] = (unsigned char)tmp;

    tmp    = uu->clock_seq;
    out[9] = (unsigned char)tmp;
    tmp >>= 8;
    out[8] = (unsigned char)tmp;

    memcpy(out + 10, uu->node, 6);
}

void uuid_unpack(const uuid_t in, struct uuid *uu) {
    const uint8_t *ptr = in;
    uint32_t       tmp;

    tmp          = *ptr++;
    tmp          = (tmp << 8) | *ptr++;
    tmp          = (tmp << 8) | *ptr++;
    tmp          = (tmp << 8) | *ptr++;
    uu->time_low = tmp;

    tmp          = *ptr++;
    tmp          = (tmp << 8) | *ptr++;
    uu->time_mid = tmp;

    tmp                     = *ptr++;
    tmp                     = (tmp << 8) | *ptr++;
    uu->time_hi_and_version = tmp;

    tmp           = *ptr++;
    tmp           = (tmp << 8) | *ptr++;
    uu->clock_seq = tmp;

    memcpy(uu->node, ptr, 6);
}
