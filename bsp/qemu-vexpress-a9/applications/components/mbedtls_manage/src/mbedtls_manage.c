#include "mbedtls_manage.h"


static int8_t mbedtls_encode(mbedtls_manage_t *self, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen)
{
    int8_t res = MBEDTLS_RES_OK;
    mbedtls_type_e type = self->type;
    switch(type)
    {
        case mbedtls_BASE64:{
            mbedtls_base64_encode(output, self->base64_max_len, (size_t *)olen, input, ilen);
        }     
        break;   
        case mbedtls_MD5:{
            mbedtls_md5_context *ctx = (mbedtls_md5_context *)&self->mbedtls_ctx;
            mbedtls_md5_starts(ctx);
            mbedtls_md5_update(ctx, (const unsigned char *)input, ilen);
            mbedtls_md5_finish(ctx, output);
            *olen = 16;
        }
        break;
        case mbedtls_SHA1:{
            mbedtls_sha1_context *ctx = (mbedtls_sha1_context *)&self->mbedtls_ctx;
            mbedtls_sha1_starts(ctx);
            mbedtls_sha1_update(ctx, (const unsigned char *)input, ilen);
            mbedtls_sha1_finish(ctx, output);
            *olen = 20;
        }
        break;
        case mbedtls_SHA256:{
            mbedtls_sha256_context *ctx = (mbedtls_sha256_context *)&self->mbedtls_ctx;
            mbedtls_sha256_starts(ctx, 0); // 0:SHA256 1:SHA244
            mbedtls_sha256_update(ctx, (const unsigned char *)input, ilen);
            mbedtls_sha256_finish(ctx, output);
            *olen = 32;
        }
        break;
        default: 
        res = MBEDTLS_RES_UNKNOWCMD;
        break;
    }
    return res;
}

static int8_t mbedtls_decode(mbedtls_manage_t *self, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen)
{
    int8_t res = MBEDTLS_RES_OK;
    mbedtls_type_e type = self->type;
    switch(type)
    {
        case mbedtls_BASE64:{
            mbedtls_base64_decode(output, self->base64_max_len, (size_t *)olen, input, ilen);
        }        
        break;
        default: 
        res = MBEDTLS_RES_UNKNOWCMD;
        break;
    }
    return res;
}

int8_t mbedtls_manage_create(mbedtls_manage_t *self, mbedtls_type_e type)
{
    int8_t res = MBEDTLS_RES_OK;
    if(self == NULL)
    {
        return MBEDTLS_RES_FAIL;
    }
    self->type = type;

    self->DoEncode = (mbedtls_encode_f)mbedtls_encode;
    self->DoDecode = (mbedtls_decode_f)mbedtls_decode;

    switch(type)
    {
        case mbedtls_BASE64:{
            self->base64_max_len = BASE64_MAX_LEN;
        }
        break;
        case mbedtls_MD5:{
            self->DoInit = (mbedtls_init_f)mbedtls_md5_init;
            self->DoDeinit = (mbedtls_deinit_f)mbedtls_md5_free;
        }
        break;
        case mbedtls_SHA1:{
            self->DoInit = (mbedtls_init_f)mbedtls_sha1_init;
            self->DoDeinit = (mbedtls_deinit_f)mbedtls_sha1_free;
        }
        break;
        case mbedtls_SHA256:{
            self->DoInit = (mbedtls_init_f)mbedtls_sha256_init;
            self->DoDeinit = (mbedtls_deinit_f)mbedtls_sha256_free;
        }
        break;
        default: 
        res = MBEDTLS_RES_UNKNOWCMD;
        break;
    }
    return res;
}

int8_t mbedtls_manage_destory(mbedtls_manage_t *self)
{
    int8_t res = MBEDTLS_RES_OK;
    if(self == NULL)
    {
        return MBEDTLS_RES_FAIL;
    }
    self->type = mbedtls_null;

    return res;
}

int8_t mbedtls_manage_control(mbedtls_manage_t *self, mbedtls_control_cmd_e cmd, char *data, uint16_t len)
{
    int8_t res = MBEDTLS_RES_OK;
    if(self == NULL || data == NULL)
    {
        return MBEDTLS_RES_FAIL;
    }

    switch(cmd)
    {
        case mbedtls_aes_set_key:{
            
        }
        break;
        case mbedtls_aes_set_vi:{
            
        }
        break;
        default: 
        res = MBEDTLS_RES_UNKNOWCMD;
        break;
    }
    return res;
}

int8_t mbedtls_manage_encode(mbedtls_manage_t *self, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen)
{
    int8_t res = MBEDTLS_RES_OK;

    if(self->DoEncode == NULL)
        return MBEDTLS_RES_FAIL;
    res = self->DoEncode(self, input, ilen, output, olen);
    return res;
}

int8_t mbedtls_manage_decode(mbedtls_manage_t *self, const uint8_t *input, uint16_t ilen, uint8_t *output, uint16_t *olen)
{
    int8_t res = MBEDTLS_RES_OK;

    if(self->DoDecode == NULL)
        return MBEDTLS_RES_FAIL;
    res = self->DoDecode(self, input, ilen, output, olen);
    return res;
}
