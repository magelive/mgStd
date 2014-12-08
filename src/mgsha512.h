#ifndef __MG_SHA512_H_
#define __MG_SHA512_H_

#define SHA512_ROUNDNUM             80 
#define SHA512_BLOCKWIDTHNUM        16 
#define SHA512_CONSTNUM             8
#define SHA512_DIGEST_LEN           128

typedef struct mg_sha512_context
{
    unsigned long long H[SHA512_CONSTNUM];
    unsigned long long K[SHA512_ROUNDNUM];
    unsigned long long M[SHA512_BLOCKWIDTHNUM];
    unsigned long long W[SHA512_ROUNDNUM]; 
    unsigned char LastString[SHA512_BLOCKWIDTHNUM*8*2];
    unsigned int LastStringLen;
    unsigned long long all_len;

}MG_SHA512_CTX;

void mg_sha512_init(MG_SHA512_CTX *ctx);
void mg_sha512_update(MG_SHA512_CTX *ctx, char *input, int input_len);
void mg_sha512_final(unsigned char output[SHA512_DIGEST_LEN], MG_SHA512_CTX *ctx);
void mg_sha512_calc(unsigned char output[SHA512_DIGEST_LEN], char *buf, int buf_len);
void mg_sha512_file(unsigned char output[SHA512_DIGEST_LEN], char *filename);

#endif

