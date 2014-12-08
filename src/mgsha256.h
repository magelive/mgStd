#ifndef __MG_SHA256_H_
#define __MG_SHA256_H_

#define SHA256_ROUNDNUM 64 
#define SHA256_BLOCKWIDTHNUM 16 
#define SHA256_CONSTNUM 8

#define SHA256_DIGEST_LEN 20

typedef struct mg_sha256_context
{
    unsigned int H[SHA256_CONSTNUM];
    unsigned int K[SHA256_ROUNDNUM];
    unsigned int M[SHA256_BLOCKWIDTHNUM]; 
    unsigned int W[SHA256_ROUNDNUM]; 
    unsigned char LastString[SHA256_BLOCKWIDTHNUM*4*2];
    unsigned int LastStringLen;
    unsigned long long all_len;
}MG_SHA256_CTX;



void mg_sha256_init(MG_SHA256_CTX *ctx);

void mg_sha256_update(MG_SHA256_CTX *ctx, char *input, int input_len);

void mg_sha256_final(unsigned char output[SHA256_DIGEST_LEN], MG_SHA256_CTX *ctx);

void mg_sha256_calc(unsigned char output[SHA256_DIGEST_LEN], char *buf, int buf_len);

void mg_sha256_file(unsigned char output[SHA256_DIGEST_LEN], char *filename);

#endif
