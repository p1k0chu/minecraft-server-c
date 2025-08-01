#pragma once

#include <openssl/err.h>

EVP_PKEY *gen_rsa_key();

/// out_data is a newly allocated array of out_size size
bool encrypt_rsa(void      **out_data,
                 size_t     *out_size,
                 const void *in_data,
                 size_t      in_size,
                 EVP_PKEY   *key);

/// out_data is a newly allocated array of out_size size
bool decrypt_rsa(void      **out_data,
                 size_t     *out_size,
                 const void *in_data,
                 size_t      in_size,
                 EVP_PKEY   *key);

