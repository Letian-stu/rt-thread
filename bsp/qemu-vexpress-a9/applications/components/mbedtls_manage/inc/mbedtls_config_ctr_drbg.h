#ifndef _MBEDTLS_CONFIG_CTR_DRBG_H_
#define _MBEDTLS_CONFIG_CTR_DRBG_H_

/* System support */
#define MBEDTLS_HAVE_ASM
//#define MBEDTLS_HAVE_TIME

/* mbed feature support */
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_ENTROPY_HARDWARE_ALT
//#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#define MBEDTLS_NO_PLATFORM_ENTROPY

/* mbed modules */
#define MBEDTLS_AES_C
#define MBEDTLS_AES_ROM_TABLES
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_ENTROPY_C
#define MBEDTLS_SHA256_C

int mbedtls_ctr_drbg_test(void);

#endif /* _MBEDTLS_CONFIG_CTR_DRBG_H_ */
