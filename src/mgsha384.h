#ifndef __MG_SHA384_H_
#define __MG_SHA384_H_

#define SHA384_ROUNDNUM         80 
#define SHA384_BLOCKWIDTHNUM    16 
#define SHA384_CONSTNUM         8
#define SHA384_DIGEST_LEN       96

typedef struct mg_sha384_context
{
    unsigned long long H[SHA384_CONSTNUM];
    unsigned long long K[SHA384_ROUNDNUM];
    unsigned long long M[SHA384_BLOCKWIDTHNUM];
    unsigned long long W[SHA384_ROUNDNUM];
    unsigned char LastString[SHA384_BLOCKWIDTHNUM*8*2];
    unsigned int LastStringLen;
    unsigned long long all_len;

}MG_SHA384_CTX;

void mg_sha384_init(MG_SHA384_CTX *ctx);
void mg_sha384_update(MG_SHA384_CTX *ctx, char *input, int input_len);
void mg_sha384_final(unsigned char output[SHA384_DIGEST_LEN], MG_SHA384_CTX *ctx);
void mg_sha384_calc(unsigned char output[SHA384_DIGEST_LEN], char *buf, int buf_len);
void mg_sha384_file(unsigned char output[SHA384_DIGEST_LEN], char *filename);

#endif


