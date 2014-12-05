#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <stdint.h>
#include <string.h>
#include <strings.h>

#include "mgsha1.h"

void mg_sha1_init(MG_SHA1_CTX *ctx)
{
    ctx->H[0] = 0x67452301;
    ctx->H[1] = 0xefcdab89;
    ctx->H[2] = 0x98badcfe;
    ctx->H[3] = 0x10325476;
    ctx->H[4] = 0xc3d2e1f0;

    ctx->K[0] = 0x5a827999;
    ctx->K[1] = 0x6ed9eba1;
    ctx->K[2] = 0x8f1bbcdc;
    ctx->K[3] = 0xca62c1d6;

    bzero(ctx->LastString, sizeof(ctx->LastString));

    ctx->LastStringLen = 0;
    ctx->all_len = 0;

}

static void mg_sha1_setW(MG_SHA1_CTX *ctx) 
{ 
    int index = 0;
    for(index = 0; index <=15;index++) 
    {   //编码方式，M的四个字节的顺序是反过来的，将其移位 
        ctx->W[index] = (ctx->M[index]>>24)|(ctx->M[index]<<24)|
            ((ctx->M[index]&0x00ff0000)>>8)|((ctx->M[index]&0x0000ff00)<<8); 

        //ctx->W[index] = ctx->M[index];
    }  

    for(index = 16;index<=79;index++) 
    { 
        unsigned int temp = ctx->W[index -16]^ctx->W[index - 14]^ctx->W[index - 8]^ctx->W[index - 3]; 
        ctx->W[index] = (temp<<1)|(temp>>(32 -1)); 
    } 
} 

static void mg_sha1_round(MG_SHA1_CTX *ctx) 
{ 
    ctx->A = ctx->H[0]; 
    ctx->B = ctx->H[1]; 
    ctx->C = ctx->H[2]; 
    ctx->D = ctx->H[3]; 
    ctx->E = ctx->H[4]; 
    int index = 0; 
    unsigned int temp; 
    for(index = 0;index <=79;index++) 
    { 
        if(index >= 0 && index <=19) 
            temp = (ctx->E) + (((ctx->B)&(ctx->C))|(~(ctx->B)&(ctx->D))) + 
                (((ctx->A)<<5)|((ctx->A)>>(32-5))) + ctx->W[index] + ctx->K[0]; 
        else if(index >= 20 && index <=39) 
            temp = (ctx->E) + (ctx->B^ctx->C^ctx->D) + 
                ((ctx->A<<5)|(ctx->A>>(32-5))) + ctx->W[index] + ctx->K[1]; 
        else if(index >= 40 && index <=59) 
            temp = ctx->E + ((ctx->B&ctx->C)|(ctx->C&ctx->D)|(ctx->B&ctx->D)) + 
                ((ctx->A<<5)|(ctx->A>>(32-5))) + ctx->W[index] + ctx->K[2]; 
        else if(index >= 60 && index <=79) 
            temp = ctx->E + (ctx->B^ctx->C^ctx->D) + ((ctx->A<<5)|(ctx->A>>(32-5))) + 
                ctx->W[index] + ctx->K[3]; 

        ctx->E = ctx->D; 
        ctx->D = ctx->C; 
        ctx->C = (ctx->B<<30)|(ctx->B>>(32-30)); 
        ctx->B = ctx->A; 
        ctx->A = temp; 
    } 

    ctx->H[0] += ctx->A; 
    ctx->H[1] += ctx->B; 
    ctx->H[2] += ctx->C; 
    ctx->H[3] += ctx->D; 
    ctx->H[4] += ctx->E; 
} 

void mg_sha1_update(MG_SHA1_CTX *ctx, char *input, int input_len)
{
    int i, process_len = 0;
    ctx->all_len += input_len;
    if (input_len+ctx->LastStringLen < 16 * 4)
    {
        //bzero(ctx->LastString, sizeof(ctx->LastString));
        memcpy(ctx->LastString+ctx->LastStringLen, input, input_len);
        ctx->LastStringLen += input_len;
        return;
    }
    else
    {
        process_len = 0;
        for (i = 0; i < ctx->LastStringLen/64; i++)
        {
            bzero(ctx->M, sizeof(ctx->M));
            memcpy(ctx->M, ctx->LastString+process_len, 64);
            process_len += 64;
            mg_sha1_setW(ctx);
            mg_sha1_round(ctx);

        }
        bzero(ctx->M, sizeof(ctx->M));
        memcpy(ctx->M, ctx->LastString + process_len, ctx->LastStringLen%64);
        memcpy(((char *)ctx->M)+ctx->LastStringLen%64, input, 64-(ctx->LastStringLen%64));
        input_len -= (64-(ctx->LastStringLen%64));
        process_len = 64 - (ctx->LastStringLen%64);
        mg_sha1_setW(ctx);
        mg_sha1_round(ctx);
    }

    for(i = 0; i < input_len/64; i++)
    {
        memcpy(ctx->M, input+process_len, 64);
        process_len += 64;
        mg_sha1_setW(ctx);
        mg_sha1_round(ctx);
    }
    bzero(ctx->LastString, sizeof(ctx->LastString));
    memcpy(ctx->LastString, input+process_len, input_len%64);
    ctx->LastStringLen = input_len%64;
    return;
}

void mg_sha1_final(unsigned char output[SHA1_DIGEST_LEN], MG_SHA1_CTX *ctx)
{
    unsigned long long realLen = ctx->all_len;
    int flag=0, LastRoundCount = 1;
    int i;
    ctx->LastString[realLen%(512/8)] = 0x80;
    if(ctx->LastStringLen <=55)            //将长度放在末尾 
    {
        flag = 1;
        for( i = 8; i >= 1; i--) 
            ctx->LastString[512/8 - i] = (char)((realLen<<3)>>((i-1)*8));   
    } 
    else 
    { 
        flag = 2;
        for( i = 1; i <= 8; i++) 
            ctx->LastString[512/8*2 - i] = (char)((realLen<<3)>>((i-1)*8));   
    }
    while(LastRoundCount <= flag)
    {
        memcpy(ctx->M,(ctx->LastString+512/8*(LastRoundCount - 1)),64);//最后的一段或是两段 
        LastRoundCount++;
        mg_sha1_setW(ctx);
        mg_sha1_round(ctx);
    }
    
    
    for(i = 0; i < 5; i++)
    {
        sprintf(output+i*8, "%08x", ctx->H[i]);
    }
    printf("output = %s\n", output);
    return; 
}

void mg_sha1_calc(unsigned char output[SHA1_DIGEST_LEN], char *buf, int buf_len)
{
    MG_SHA1_CTX mg_sha1_ctx;
    mg_sha1_init(&mg_sha1_ctx);
    mg_sha1_update(&mg_sha1_ctx, buf, buf_len);
    mg_sha1_final(output, &mg_sha1_ctx);
}

void mg_sha1_file(unsigned char output[SHA1_DIGEST_LEN], char *filename)
{
    if (!filename)
        return;
    printf("filename = %s\n", filename);
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("file open error!\n");
        return ;
    }
    char buf[1024];
    bzero(buf, 1024);
    MG_SHA1_CTX mg_sha1_ctx;
    mg_sha1_init(&mg_sha1_ctx);
    while(fgets(buf, 1024, fp) != NULL)
    {
        mg_sha1_update(&mg_sha1_ctx, buf, strlen(buf));
        bzero(buf, 1024);
    }
    mg_sha1_final(output, &mg_sha1_ctx);
    fclose(fp);
    return ;
}
