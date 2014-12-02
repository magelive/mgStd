#include <stdio.h>
#include <stdlib.h>
#include "mgmd5.h"

void md5_decode(unsigned char *output, unsigned char * input, int len)
{
    MG_MD5_CTX context;
    mg_MD5Init(&context);
    mg_MD5Update(&context, input, len);
    /*
       mg_MD5_Update(...);
       ...
     */
    mg_MD5Final(output, &context);
}  

int main(int argc, char *argv[])
{
    unsigned char *buf = malloc(128);
    unsigned char *filebuf = malloc(32);
    memset(buf, 0, 128);
    memset(buf, 0, 32);
    mg_md5_calc(buf, "topsec", 6);
    mg_md5_file(filebuf, "./mgmd5.c");
    unsigned char outbuf[64];
    unsigned char fileoutbuf[64];
    memset(outbuf, 0, 1024);
    int i;
    for( i = 0; i < 16; i++)
    {
        sprintf(fileoutbuf+i*2, "%02x", filebuf[i]);
        sprintf(outbuf+i*2, "%02x", buf[i]);
    }
    printf("fileoutbuf = %s, outbuf = %s\n",fileoutbuf, outbuf);
    free(buf);
    free(filebuf);
}
