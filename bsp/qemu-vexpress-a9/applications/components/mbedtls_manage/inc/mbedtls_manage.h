#ifndef _MBEDTLS_MANAGE_H_
#define _MBEDTLS_MANAGE_H_

/* -------------------------------------------------------------------------- */
/*                                   include                                  */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdint.h>

#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha1.h"
#include "mbedtls/base64.h"

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*                                  redefine                                  */
/* -------------------------------------------------------------------------- */
#define BASE64_MAX_LEN 1024

typedef union mbedtls_type mbedtls_type_u;
typedef struct mbedtls_manage mbedtls_manage_t;

typedef void (*mbedtls_init_f)(mbedtls_type_u *ctx);
typedef void (*mbedtls_deinit_f)(mbedtls_type_u *ctx);
typedef int8_t (*mbedtls_encode_f)(mbedtls_manage_t *ctx, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen);
typedef int8_t (*mbedtls_decode_f)(mbedtls_manage_t *ctx, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen);
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  variable                                  */
/* -------------------------------------------------------------------------- */
enum{
    MBEDTLS_RES_OK = 0x00,
    MBEDTLS_RES_FAIL,
    MBEDTLS_RES_UNKNOWCMD,
};

typedef enum{
    mbedtls_null = 0x00,
    mbedtls_BASE64,
    mbedtls_MD5,
    mbedtls_SHA1,
    mbedtls_SHA256,
    mbedtls_AES,
}mbedtls_type_e;

typedef enum{
    mbedtls_aes_set_key = 0x00,
    mbedtls_aes_set_vi,
}mbedtls_control_cmd_e;

union mbedtls_type
{
    mbedtls_md5_context md5_ctx;
    mbedtls_sha1_context sha1_ctx;
    mbedtls_sha256_context sha256_ctx;
};

struct mbedtls_manage
{
    mbedtls_type_u mbedtls_ctx;
    mbedtls_type_e type;
    uint32_t base64_max_len;

    mbedtls_init_f DoInit;
    mbedtls_deinit_f DoDeinit;
    mbedtls_encode_f DoEncode;
    mbedtls_decode_f DoDecode;
};
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                  function                                  */
/* -------------------------------------------------------------------------- */
int8_t mbedtls_manage_create(mbedtls_manage_t *self, mbedtls_type_e type);

int8_t mbedtls_manage_destory(mbedtls_manage_t *self);

int8_t mbedtls_manage_control(mbedtls_manage_t *self, mbedtls_control_cmd_e cmd, char *data, uint16_t len);

int8_t mbedtls_manage_encode(mbedtls_manage_t *self, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen);

int8_t mbedtls_manage_decode(mbedtls_manage_t *self, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen);
/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
