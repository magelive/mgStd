#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <stdint.h>
#include <string.h>

#include "mgsha1.h"


int main(int argc, char *argv[])
{
    /*
    FILE *fp = fopen("./mgsha1_test.c", "rb");
    if (fp == NULL)
    {
        printf("file open error!\n");
        return 0;
    }
    char buf[1024];
    unsigned char outbuf[64];
    bzero(buf, 1024);
    bzero(outbuf, 64);
    
    MG_SHA1_CTX mg_sha1_ctx;
    mg_sha1_init(&mg_sha1_ctx);
    while(fgets(buf, 1024, fp) != NULL)
    {
        mg_sha1_update(&mg_sha1_ctx, buf, strlen(buf));
        bzero(buf, 1024);
    }
    mg_sha1_final(outbuf, &mg_sha1_ctx);
    fclose(fp);
    printf("file sha1_dat.data sha1 : %s\n", outbuf);
    */

    unsigned char outbuf[64];
    bzero(outbuf, 64);
    uint64_t t = mg_get_tick_time_us();
    mg_sha1_file(outbuf, "./mgsha1_test.c");
    uint64_t n = mg_get_tick_time_us();
    printf("sha1 : %s, time : %d us\n", outbuf, n-t);
    return 0;
}
