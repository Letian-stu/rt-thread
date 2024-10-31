#include "mbedtls/config.h"
#include <stdio.h>
#include "string.h"
#include "mbedtls/md.h"
#include "mbedtls_config_sha_x.h"

#define LOG_TAG     "md"
#define LOG_LVL     LOG_LVL_DBG  
#include <ulog.h>         

int mbedtls_shax_mdx_test(mbedtls_md_type_t md_type)
{
    int len;
    int ret;
    const char message[] = "hello tian nb plus";
    unsigned char digest[1024];
    
    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;

    LOG_I("message is:%s", message);

    /* 1. init mbedtls_md_context_t structure */
    mbedtls_md_init(&ctx);
    
    /* 2. get md info structure pointer */
    info = mbedtls_md_info_from_type(md_type);
    
    /* 3. setup md info structure */
    ret = mbedtls_md_setup(&ctx, info, 0);
    if (ret != 0) {
        goto exit;
    }
    
    /* 4. start */
    ret = mbedtls_md_starts(&ctx);
     if (ret != 0) {
        goto exit;
    }
     
    /* 5. update */
    ret = mbedtls_md_update(&ctx, (unsigned char *)message, strlen(message));
    if (ret != 0) {
        goto exit;
    }
     
    /* 6. finish */
    ret = mbedtls_md_finish(&ctx, digest);
    if (ret != 0) {
        goto exit;
    }
    
    /* show */
    LOG_I("%s digest context is:", mbedtls_md_get_name(info));
    len = mbedtls_md_get_size(info);
    ulog_hexdump(LOG_TAG, 16, digest, len);

    exit:
    /* 7. free */
    mbedtls_md_free(&ctx);
    
    return ret;
}

