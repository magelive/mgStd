#ifndef _HMAC_MD5_INC
#define _HMAC_MD5_INC

void hmac_md5(unsigned char *output, 
        const unsigned char *input, size_t input_len,
        const unsigned char *key, size_t key_len);

#endif

