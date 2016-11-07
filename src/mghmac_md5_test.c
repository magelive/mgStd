#include <stdio.h>
#include <stdlib.h>
#include "mghmac_md5.h"

void test()
{
    int i = 0;
    char *key = "123456";
    char *md5_str = "abcdefghijklmnopqrstuvwxyz";
    unsigned char out_str[16] = {0x00};
    unsigned char out[64] = {0x00};
    hmac_md5(out_str, md5_str, strlen(md5_str), key, strlen(key));
    for(i = 0; i < 16; i++)
    {
        sprintf(out+i*2, "%02x", (unsigned char)out_str[i]);
    }
    printf("out : %s\n", out);
}

int main()
{
    test();
}

