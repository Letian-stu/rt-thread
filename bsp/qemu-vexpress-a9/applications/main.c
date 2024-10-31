/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020/12/31     Bernard      Add license info
 */

#include <stdint.h>
#include <stdio.h>
#include <rtthread.h>

#include "drv_log.h"

#include "attr.h"
#include "attr_pid_table.h"


#include "mbedtls/cipher.h"
#include "mbedtls/md.h"

#include "mbedtls_manage.h"
#include "mbedtls_config_aes.h"
#include "mbedtls_config_dhm.h"
#include "mbedtls_config_ecdh.h"
#include "mbedtls_config_ecdsa.h"
#include "mbedtls_config_gcm.h"
#include "mbedtls_config_hmac.h"
#include "mbedtls_config_rsa_sign.h"
#include "mbedtls_config_rsa.h"
#include "mbedtls_config_sha_x.h"
#include "mbedtls_config_x509.h"


#define LOG_TAG     "app"
#define LOG_LVL     LOG_LVL_DBG  
#include <ulog.h>                 


void mbedtls_demo(void)
{
    /* 0. sha1 sha256 sha512 test */ //不带key
    // mbedtls_shax_mdx_test(MBEDTLS_MD_MD5);
    // mbedtls_shax_mdx_test(MBEDTLS_MD_SHA1);
    // mbedtls_shax_mdx_test(MBEDTLS_MD_SHA256);
    // mbedtls_shax_mdx_test(MBEDTLS_MD_SHA512);
    
    /* 1. crt_drbg test ???*/
    //mbedtls_ctr_drbg_test();

    /* 2. aes test */
    mbedtls_aes_test();

    // /* 3. hamc test */ //带key
    //mbedtls_hmac_test(MBEDTLS_MD_SHA256); 
    
    // /* 4.gcm test */
    //gcm_test(MBEDTLS_CIPHER_AES_128_GCM); //!
    
    // /* 5. rsa test */
    //mbedtls_rsa_test();
    
    // /* 6. dh test */
    // mbedtls_dhm_test();
    
    // /* 7. ecdh test */
    // mbedtls_ecdh_test();
    
    // /* 8. rsa sign test */
    // mbedtls_rsa_sign_test();
    
    // /* 9. ecdsa sign test */
    // mbedtls_ecdsa_test();
    
    // /* 10. x509 test */
    // mbedtls_x509_test();

}

int main(int argc, char *argv[]) 
{
    rt_kprintf("Hello RT-Thread!\n");

	mbedtls_demo();
	return 0;
}

