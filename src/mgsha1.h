#ifndef __MG_SHA1__
#define __MG_SHA1__

#define SHA1_DIGEST_LEN 20

typedef struct mg_sha1_context
{
    unsigned int H[5];
    unsigned int K[4];

    unsigned int M[16];
    unsigned int W[80];

    unsigned int A;
    unsigned int B;
    unsigned int C;
    unsigned int D;
    unsigned int E;
    unsigned char LastString[512/8*2];
    unsigned int LastStringLen;
    unsigned long long all_len;

}MG_SHA1_CTX;

void mg_sha1_init(MG_SHA1_CTX *ctx);
void mg_sha1_update(MG_SHA1_CTX *ctx, char *input, int input_len);
void mg_sha1_final(unsigned char output[SHA1_DIGEST_LEN], MG_SHA1_CTX *ctx);
void mg_sha1_file(unsigned char output[SHA1_DIGEST_LEN], char *filename);

#endif
