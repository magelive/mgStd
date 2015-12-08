#ifndef __MG_CRC32_H_
#define __MG_CRC32_H_

#include <stdint.h>

#define CRC32_DIGEST_LEN   8 

typedef struct mg_crc32_context
{
    uint32_t crc_tab[256];
    unsigned long mask;
}MG_CRC32_CTX;

void mg_crc32_init(MG_CRC32_CTX *ctx);

void mg_crc32_update(MG_CRC32_CTX *ctx, unsigned char *block, unsigned int length);

void mg_crc32_final(uint32_t *result, MG_CRC32_CTX *ctx);

uint32_t mg_checksum_crc32(unsigned char output[CRC32_DIGEST_LEN], unsigned char *block, unsigned int length);

uint32_t mg_checksum_crc32_file(unsigned char output[CRC32_DIGEST_LEN], char *filename);

#endif
