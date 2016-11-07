#include <stdio.h>
#include <stdlib.h>
#include "mgmd5.h"

void hmac_md5(unsigned char *output, 
        const unsigned char *input, size_t input_len,
        const unsigned char *key, size_t key_len)
{
    unsigned char hmac_key[64] = {0x00};
    unsigned char k_ipad[64] = {0x00};
    unsigned char k_opad[64] = {0x00};

    if (!output)
        abort();

    if (!input)
        input_len = 0;

    if (!key)
        key_len = 0;

    if (key_len > 64)
        mg_md5_calc(hmac_key, key, key_len);
    else
        memcpy(hmac_key, key, key_len);

    int i = 0;
    for( i = 0; i < 64; i++ )
    {
        k_ipad[i] = hmac_key[i] ^ 0x36;
        k_opad[i] = hmac_key[i] ^ 0x5c;
    }
    MG_MD5_CTX context1;
    mg_MD5Init(&context1);
    mg_MD5Update(&context1, k_ipad, 64);
    mg_MD5Update(&context1, input, input_len);
    mg_MD5Final(hmac_key, &context1);
    
    MG_MD5_CTX context2;
    mg_MD5Init(&context2);
    mg_MD5Update(&context2, k_opad, 64);
    mg_MD5Update(&context2, hmac_key, 16);
    mg_MD5Final(output, &context2);
    return;
}


