
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "mgcrc32.h"

void mg_crc32_init(MG_CRC32_CTX *ctx)
{
    unsigned long crc, poly;
    int i, j;

    //poly = 0x04C11DB7L;//0x04C11DB7L;//0xEDB88320L;
    poly = 0xEDB88320L;
    for (i = 0; i < 256; i++)
    {
        crc = i;
        for (j = 8; j > 0; j--)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ poly;
            }
            else
            {
                crc >>= 1;
            }
        }
        ctx->crc_tab[i] = crc;
    }
    ctx->mask = 0xFFFFFFFF;
}

void mg_crc32_update(MG_CRC32_CTX *ctx, unsigned char *block, unsigned int length)
{
    register unsigned long mask;
    unsigned long i;

    mask = ctx->mask;
    for (i = 0; i < length; i++)
    {
        mask = ((mask >> 8) & 0x00FFFFFF) ^ ctx->crc_tab[(mask ^ *block++) & 0xFF];
    }
    ctx->mask = mask;
}

void mg_crc32_final(uint32_t *result, MG_CRC32_CTX *ctx)
{
    *result = (ctx->mask ^ 0xFFFFFFFF);
}

uint32_t mg_checksum_crc32(unsigned char output[CRC32_DIGEST_LEN], unsigned char *block, unsigned int length)
{
    uint32_t result = 0;
    MG_CRC32_CTX crc32_ctx;
    mg_crc32_init(&crc32_ctx);
    mg_crc32_update(&crc32_ctx, block, length);
    mg_crc32_final(&result, &crc32_ctx);
    sprintf(output, "%08x", result);
    return result;
}

uint32_t mg_checksum_crc32_file(unsigned char output[CRC32_DIGEST_LEN], char *filename)
{
    uint32_t result = 0;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        perror("fopen : ");
        return 0;
    }
    char buf[1024];
    bzero(buf, 1024);
    MG_CRC32_CTX mg_crc32_ctx;
    mg_crc32_init(&mg_crc32_ctx);
    while(fgets(buf, 1024, fp) != NULL)
    {
        mg_crc32_update(&mg_crc32_ctx, buf, strlen(buf));
        bzero(buf, 1024);
    }
    mg_crc32_final(&result, &mg_crc32_ctx);
    fclose(fp);
    sprintf(output, "%08x", result);
    return result;
}

