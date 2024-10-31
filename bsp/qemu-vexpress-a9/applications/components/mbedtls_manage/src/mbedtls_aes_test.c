#include "mbedtls/config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "mbedtls/cipher.h"

#define LOG_TAG     "aes"
#define LOG_LVL     LOG_LVL_DBG  
#include <ulog.h>                 


uint8_t key[16] = "1234567887654321";

uint8_t iv[16] = "1234567887654321";

char encode_ecb_data[17] = "hello tian hello";

char encode_data[32] = "hello tian hello world nbbb";

size_t encrypt_len = 0;
char encrypt_data[1024] = {0};

size_t decode_len = 0;
char decode_data[1024] = {0};


int mbedtls_aes_encode(mbedtls_cipher_type_t cipher_type, mbedtls_cipher_padding_t padding_mode, char *encode_data, size_t ilen, char *encrypt_data, size_t *olen)
{
    int ret;

    mbedtls_cipher_context_t ctx;
    mbedtls_cipher_info_t *info;
    
    mbedtls_cipher_init(&ctx);
    
    info = mbedtls_cipher_info_from_type(cipher_type);
    
    ret = mbedtls_cipher_setup(&ctx, info);
    if (ret != 0) {
        LOG_I("mbedtls_cipher_setup err %d",ret);
        goto exit;
    }
    
    ret = mbedtls_cipher_setkey(&ctx, key, sizeof(key) * 8, MBEDTLS_ENCRYPT);
    if (ret != 0) {
        LOG_I("mbedtls_cipher_setkey err %d",ret);
        goto exit;
    }
    
    if(cipher_type >= MBEDTLS_CIPHER_AES_128_CBC  && cipher_type <= MBEDTLS_CIPHER_AES_256_CBC)
    {
        ret = mbedtls_cipher_set_padding_mode(&ctx, padding_mode);
        if (ret != 0) {
            LOG_I("mbedtls_cipher_set_padding_mode err %d",ret);
        }        
    }

    if(cipher_type > MBEDTLS_CIPHER_AES_256_ECB)
    {
        ret = mbedtls_cipher_set_iv(&ctx, iv, sizeof(iv));
        if (ret != 0) {
            LOG_I("mbedtls_cipher_set_iv err %d",ret);
        }          
    }
      
    ret = mbedtls_cipher_crypt(&ctx, iv, sizeof(iv), encode_data, ilen, encrypt_data, olen);
    if (ret != 0) {
        LOG_I("mbedtls_cipher_crypt err %d",ret);
        goto exit;
    }

    exit:
    mbedtls_cipher_free(&ctx);
    
    return ret;
}

int mbedtls_aes_decode(mbedtls_cipher_type_t cipher_type, mbedtls_cipher_padding_t padding_mode, char *encrypt_data, size_t ilen, char *decode_data, size_t *olen)
{
    int ret;

    mbedtls_cipher_context_t ctx;
    mbedtls_cipher_info_t *info;
    
    mbedtls_cipher_init(&ctx);
    
    info = mbedtls_cipher_info_from_type(cipher_type);
    
    ret = mbedtls_cipher_setup(&ctx, info);
    if (ret != 0) {
        LOG_I("mbedtls_cipher_setup err %d",ret);
        goto exit;
    }
    
    ret = mbedtls_cipher_setkey(&ctx, key, sizeof(key) * 8, MBEDTLS_DECRYPT);
    if (ret != 0) {
        LOG_I("mbedtls_cipher_setkey err %d",ret);
        goto exit;
    }
    
    if(cipher_type >= MBEDTLS_CIPHER_AES_128_CBC  && cipher_type <= MBEDTLS_CIPHER_AES_256_CBC)
    {
        ret = mbedtls_cipher_set_padding_mode(&ctx, padding_mode);
        if (ret != 0) {
            LOG_I("mbedtls_cipher_set_padding_mode err %d",ret);
            goto exit;
        }        
    }

    if(cipher_type > MBEDTLS_CIPHER_AES_256_ECB)
    {
        ret = mbedtls_cipher_set_iv(&ctx, iv, sizeof(iv));
        if (ret != 0) {
            LOG_I("mbedtls_cipher_set_iv err %d",ret);
            goto exit;
        }          
    }   
    
    ret = mbedtls_cipher_crypt(&ctx, iv, sizeof(iv), encrypt_data, ilen, decode_data, olen);
    if (ret != 0) {
        LOG_I("mbedtls_cipher_crypt err %d",ret);
        goto exit;
    }

    exit:
    mbedtls_cipher_free(&ctx);
    
    return ret;
}

void mbedtls_aes_test(void)
{
    LOG_I("=============================AES_128_ECB==============================");
    memset(encrypt_data, 0x00, sizeof(encrypt_data));
    memset(decode_data, 0x00, sizeof(decode_data));

    LOG_I("encode_data %d:[%s]", strlen(encode_ecb_data), encode_ecb_data);
    LOG_HEX(LOG_TAG, 16, encode_ecb_data, strlen(encode_ecb_data));
    mbedtls_aes_encode(MBEDTLS_CIPHER_AES_128_ECB, MBEDTLS_PADDING_NONE, encode_ecb_data, 16, encrypt_data, &encrypt_len);
    LOG_I("encrypt_data %d:[%s]", encrypt_len, encrypt_data);
    LOG_HEX(LOG_TAG, 16, encrypt_data, encrypt_len);
    mbedtls_aes_decode(MBEDTLS_CIPHER_AES_128_ECB, MBEDTLS_PADDING_NONE, encrypt_data, encrypt_len, decode_data, &decode_len);
    LOG_I("encrypt_data %d:[%s]", decode_len, decode_data);
    LOG_HEX(LOG_TAG, 16, decode_data, decode_len);

    LOG_I("=============================AES_128_CBC==============================");

    memset(encrypt_data, 0x00, sizeof(encrypt_data));
    memset(decode_data, 0x00, sizeof(decode_data));

    LOG_I("encode_data %d:[%s]", strlen(encode_data), encode_data);
    LOG_HEX(LOG_TAG, 16, encode_data, strlen(encode_data));
    mbedtls_aes_encode(MBEDTLS_CIPHER_AES_128_CBC, MBEDTLS_PADDING_PKCS7, encode_data, strlen(encode_data), encrypt_data, &encrypt_len);
    LOG_I("encrypt_data %d:[%s]", encrypt_len, encrypt_data);
    LOG_HEX(LOG_TAG, 16, encrypt_data, encrypt_len);
    mbedtls_aes_decode(MBEDTLS_CIPHER_AES_128_CBC, MBEDTLS_PADDING_PKCS7, encrypt_data, encrypt_len, decode_data, &decode_len);
    LOG_I("encrypt_data %d:[%s]", decode_len, decode_data);
    LOG_HEX(LOG_TAG, 16, decode_data, decode_len);

    LOG_I("=============================AES_128_CFB128==============================");

    memset(encrypt_data, 0x00, sizeof(encrypt_data));
    memset(decode_data, 0x00, sizeof(decode_data));

    LOG_I("encode_data %d:[%s]", strlen(encode_data), encode_data);
    LOG_HEX(LOG_TAG, 16, encode_data, strlen(encode_data));
    mbedtls_aes_encode(MBEDTLS_CIPHER_AES_128_CFB128, MBEDTLS_PADDING_PKCS7, encode_data, strlen(encode_data), encrypt_data, &encrypt_len);
    LOG_I("encrypt_data %d:[%s]", encrypt_len, encrypt_data);
    LOG_HEX(LOG_TAG, 16, encrypt_data, encrypt_len);
    mbedtls_aes_decode(MBEDTLS_CIPHER_AES_128_CFB128, MBEDTLS_PADDING_PKCS7, encrypt_data, encrypt_len, decode_data, &decode_len);
    LOG_I("encrypt_data %d:[%s]", decode_len, decode_data);
    LOG_HEX(LOG_TAG, 16, decode_data, decode_len);

    LOG_I("=============================AES_128_CTR==============================");

    memset(encrypt_data, 0x00, sizeof(encrypt_data));
    memset(decode_data, 0x00, sizeof(decode_data));

    LOG_I("encode_data %d:[%s]", strlen(encode_data), encode_data);
    LOG_HEX(LOG_TAG, 16, encode_data, strlen(encode_data));
    mbedtls_aes_encode(MBEDTLS_CIPHER_AES_128_CTR, MBEDTLS_PADDING_PKCS7, encode_data, strlen(encode_data), encrypt_data, &encrypt_len);
    LOG_I("encrypt_data %d:[%s]", encrypt_len, encrypt_data);
    LOG_HEX(LOG_TAG, 16, encrypt_data, encrypt_len);
    mbedtls_aes_decode(MBEDTLS_CIPHER_AES_128_CTR, MBEDTLS_PADDING_PKCS7, encrypt_data, encrypt_len, decode_data, &decode_len);
    LOG_I("encrypt_data %d:[%s]", decode_len, decode_data);
    LOG_HEX(LOG_TAG, 16, decode_data, decode_len);

    LOG_I("=============================AES_128_GCM==============================");

    memset(encrypt_data, 0x00, sizeof(encrypt_data));
    memset(decode_data, 0x00, sizeof(decode_data));

    LOG_I("encode_data %d:[%s]", strlen(encode_data), encode_data);
    LOG_HEX(LOG_TAG, 16, encode_data, strlen(encode_data));
    mbedtls_aes_encode(MBEDTLS_CIPHER_AES_128_GCM, MBEDTLS_PADDING_PKCS7, encode_data, strlen(encode_data), encrypt_data, &encrypt_len);
    LOG_I("encrypt_data %d:[%s]", encrypt_len, encrypt_data);
    LOG_HEX(LOG_TAG, 16, encrypt_data, encrypt_len);
    mbedtls_aes_decode(MBEDTLS_CIPHER_AES_128_GCM, MBEDTLS_PADDING_PKCS7, encrypt_data, encrypt_len, decode_data, &decode_len);
    LOG_I("encrypt_data %d:[%s]", decode_len, decode_data);
    LOG_HEX(LOG_TAG, 16, decode_data, decode_len);
}


