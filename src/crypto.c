#include "./crypto.h"

#include "utils.h"

#include <openssl/evp.h>
#include <openssl/pem.h>

#define HANDLE_ERROR(evp_pkey_ctx, return_value) \
    {                                            \
        ERR_print_errors_fp(stderr);             \
        EVP_PKEY_CTX_free((evp_pkey_ctx));       \
        return (return_value);                   \
    }

EVP_PKEY *gen_rsa_key() {
    EVP_PKEY_CTX *ctx  = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    EVP_PKEY     *pkey = NULL;

    if (ctx == NULL) HANDLE_ERROR(ctx, NULL);

    if (EVP_PKEY_keygen_init(ctx) <= 0) HANDLE_ERROR(ctx, NULL);
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) HANDLE_ERROR(ctx, NULL);
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) HANDLE_ERROR(ctx, NULL);

    EVP_PKEY_CTX_free(ctx);

    return pkey;
}

bool encrypt_rsa(void **const      out_data,
                 size_t *const     out_size,
                 const void *const in_data,
                 const size_t      in_size,
                 EVP_PKEY *const   key) {
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, NULL);
    if (ctx == NULL) {
        return false;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) HANDLE_ERROR(ctx, false);

    if (EVP_PKEY_encrypt(ctx, NULL, out_size, in_data, in_size) <= 0) HANDLE_ERROR(ctx, false);

    *out_data = OPENSSL_malloc(*out_size);
    if (*out_data == NULL) {
        COOL_PERROR("encrypt_rsa: malloc");
        HANDLE_ERROR(ctx, false);
    }

    if (EVP_PKEY_encrypt(ctx, *out_data, out_size, in_data, in_size) <= 0) HANDLE_ERROR(ctx, false);

    EVP_PKEY_CTX_free(ctx);

    return true;
}

bool decrypt_rsa(void **const      out_data,
                 size_t *const     out_size,
                 const void *const in_data,
                 const size_t      in_size,
                 EVP_PKEY *const   key) {
    EVP_PKEY_CTX *ctx;

    ctx = EVP_PKEY_CTX_new(key, NULL);
    if (ctx == NULL) return false;

    if (EVP_PKEY_decrypt_init(ctx) <= 0) HANDLE_ERROR(ctx, false);
    if (EVP_PKEY_decrypt(ctx, NULL, out_size, in_data, in_size) <= 0) HANDLE_ERROR(ctx, false);

    *out_data = OPENSSL_malloc(*out_size);
    if (*out_data == NULL) {
        COOL_PERROR("decrypt_rsa: malloc");
        HANDLE_ERROR(ctx, false);
    }

    if (EVP_PKEY_decrypt(ctx, *out_data, out_size, in_data, in_size) <= 0) HANDLE_ERROR(ctx, false);

    EVP_PKEY_CTX_free(ctx);

    return true;
}

