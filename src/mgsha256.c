#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "mgsha256.h"

void mg_sha256_init(MG_SHA256_CTX *ctx)
{
    ctx->H[0] = 0x6a09e667;
    ctx->H[1] = 0xbb67ae85;
    ctx->H[2] = 0x3c6ef372;
    ctx->H[3] = 0xa54ff53a;
    ctx->H[4] = 0x510e527f;
    ctx->H[5] = 0x9b05688c;
    ctx->H[6] = 0x1f83d9ab;
    ctx->H[7] = 0x5be0cd19;

    ctx->K[0] = 0x428a2f98; ctx->K[1] = 0x71374491; ctx->K[2] = 0xb5c0fbcf; ctx->K[3] = 0xe9b5dba5;
    ctx->K[4] = 0x3956c25b; ctx->K[5] = 0x59f111f1; ctx->K[6] = 0x923f82a4; ctx->K[7] = 0xab1c5ed5;
 
    ctx->K[8] = 0xd807aa98; ctx->K[9] = 0x12835b01; ctx->K[10] = 0x243185be; ctx->K[11] = 0x550c7dc3;
    ctx->K[12] = 0x72be5d74; ctx->K[13] = 0x80deb1fe; ctx->K[14] = 0x9bdc06a7; ctx->K[15] = 0xc19bf174;
 
    ctx->K[16] = 0xe49b69c1; ctx->K[17] = 0xefbe4786; ctx->K[18] = 0x0fc19dc6; ctx->K[19] = 0x240ca1cc;
    ctx->K[20] = 0x2de92c6f; ctx->K[21] = 0x4a7484aa; ctx->K[22] = 0x5cb0a9dc; ctx->K[23] = 0x76f988da;
 
    ctx->K[24] = 0x983e5152; ctx->K[25] = 0xa831c66d; ctx->K[26] = 0xb00327c8; ctx->K[27] = 0xbf597fc7;
    ctx->K[28] = 0xc6e00bf3; ctx->K[29] = 0xd5a79147; ctx->K[30] = 0x06ca6351; ctx->K[31] = 0x14292967;
 
    ctx->K[32] = 0x27b70a85; ctx->K[33] = 0x2e1b2138; ctx->K[34] = 0x4d2c6dfc; ctx->K[35] = 0x53380d13;
    ctx->K[36] = 0x650a7354; ctx->K[37] = 0x766a0abb; ctx->K[38] = 0x81c2c92e; ctx->K[39] = 0x92722c85;
 
    ctx->K[40] = 0xa2bfe8a1; ctx->K[41] = 0xa81a664b; ctx->K[42] = 0xc24b8b70; ctx->K[43] = 0xc76c51a3;
    ctx->K[44] = 0xd192e819; ctx->K[45] = 0xd6990624; ctx->K[46] = 0xf40e3585; ctx->K[47] = 0x106aa070;
 
    ctx->K[48] = 0x19a4c116; ctx->K[49] = 0x1e376c08; ctx->K[50] = 0x2748774c; ctx->K[51] = 0x34b0bcb5; 
    ctx->K[52] = 0x391c0cb3; ctx->K[53] = 0x4ed8aa4a; ctx->K[54] = 0x5b9cca4f; ctx->K[55] = 0x682e6ff3;
 
    ctx->K[56] = 0x748f82ee; ctx->K[57] = 0x78a5636f; ctx->K[58] = 0x84c87814; ctx->K[59] = 0x8cc70208;
    ctx->K[60] = 0x90befffa; ctx->K[61] = 0xa4506ceb; ctx->K[62] = 0xbef9a3f7; ctx->K[63] = 0xc67178f2;
    
	ctx->LastStringLen = 0;
    ctx->all_len = 0;

}

static unsigned int mg_sha256_shift_num3(unsigned int input, int OpOne, int OpTwo, int Opthree) 
{ 
    unsigned int Ans = 0 ;  
    Ans ^= ((input >> OpOne)|(input<<(32-OpOne))); 
    Ans ^= ((input >> OpTwo)|(input<<(32-OpTwo))); 
    if(Opthree > 0) 
        Ans ^= ((input >> Opthree)|(input<<(32-Opthree))); 
    else 
    { 
        Opthree = 0 - Opthree; 
        Ans ^= (input >> Opthree); 
    } 
    return Ans; 
}

static void mg_sha256_setw(MG_SHA256_CTX *ctx) 
{
    int index = 0; 
    
    for(index = 0; index <= 15;index++) 
    { 
        ctx->W[index] = (ctx->M[index]>>24) | (ctx->M[index]<<24) |
			((ctx->M[index]&0x00ff0000)>>8) | ((ctx->M[index]&0x0000ff00)<<8); 
    }  
    unsigned int s0 = 0, s1 = 0; 
    for(index = 16; index < RoundNum; index++) 
    { 
        s0 = 0; 
        s1 = 0; 
        s0 = mg_sha256_shift_num3(ctx->W[index-2],17,19,-10); 
        s1 = mg_sha256_shift_num3(ctx->W[index-15],7,18,-3); 
        ctx->W[index] = (s0 + ctx->W[index-7] + s1 + ctx->W[index-16]); 
    } 
} 

static void mg_sha256_round(MG_SHA256_CTX *ctx) 
{ 
    unsigned int A,B,C,D,E,F,G,I; 
    A = ctx->H[0]; 
    B = ctx->H[1]; 
    C = ctx->H[2]; 
    D = ctx->H[3]; 
    E = ctx->H[4]; 
    F = ctx->H[5]; 
    G = ctx->H[6]; 
    I = ctx->H[7]; 

    int index = 0; 
    unsigned int t1 = 0, t2= 0, s0 = 0, s1 = 0; 
    for(index = 0;index < RoundNum ;index++) 
    { 
        s0 = 0; 
        s1 = 0; 
        s0 = mg_sha256_shift_num3(A, 2, 13, 22); 
        t2 = s0 + ((A&B)^(A&C)^(B&C)); 
        s1 = mg_sha256_shift_num3(E, 6, 11, 25); 
        t1 = I + s1 + ((E&F)^((~E)&G)) + ctx->K[index] + ctx->W[index]; 

        I = G; 
        G = F; 
        F = E; 
        E = D + t1; 
        D = C; 
        C = B; 
        B = A; 
        A = t1 + t2; 
    } 

    ctx->H[0]+=A; 
    ctx->H[1]+=B; 
    ctx->H[2]+=C; 
    ctx->H[3]+=D; 
    ctx->H[4]+=E; 
    ctx->H[5]+=F; 
    ctx->H[6]+=G; 
    ctx->H[7]+=I; 
}

void mg_sha256_update(MG_SHA256_CTX *ctx, char *input, int input_len)
{
    int i, process_len = 0;
    ctx->all_len += input_len;
    if (input_len+ctx->LastStringLen < 16 * 4)
    {
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
            mg_sha256_setw(ctx);
            mg_sha256_round(ctx);

        }
        bzero(ctx->M, sizeof(ctx->M));
        memcpy(ctx->M, ctx->LastString + process_len, ctx->LastStringLen%64);
        memcpy(((char *)ctx->M)+ctx->LastStringLen%64, input, 64-(ctx->LastStringLen%64));
        input_len -= (64-(ctx->LastStringLen%64));
        process_len = 64 - (ctx->LastStringLen%64);
        mg_sha256_setw(ctx);
        mg_sha256_round(ctx);
    }

    for(i = 0; i < input_len/64; i++)
    {
        memcpy(ctx->M, input+process_len, 64);
        process_len += 64;
        mg_sha256_setw(ctx);
        mg_sha256_round(ctx);
    }
    bzero(ctx->LastString, sizeof(ctx->LastString));
    memcpy(ctx->LastString, input+process_len, input_len%64);
    ctx->LastStringLen = input_len%64;
    return;
}

void mg_sha256_final(unsigned char output[SHA256_DIGEST_LEN], MG_SHA256_CTX *ctx)
{
    unsigned long long realLen = ctx->all_len;
    int flag=0, LastRoundCount = 1;
    int i;
    ctx->LastString[realLen%(512/8)] = 0x80;
    if(ctx->LastStringLen <=55)  
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
        mg_sha256_setw(ctx);
        mg_sha256_round(ctx);
    }
    
    for(i = 0; i < ConstNum; i++)
    {
        sprintf(output+i*8, "%08x", ctx->H[i]);
    }
    return; 
}

void mg_sha256_calc(unsigned char output[SHA256_DIGEST_LEN], char *buf, int buf_len)
{
    MG_SHA256_CTX mg_sha256_ctx;
    mg_sha256_init(&mg_sha256_ctx);
    mg_sha256_update(&mg_sha256_ctx, buf, buf_len);
    mg_sha256_final(output, &mg_sha256_ctx);
}

void mg_sha256_file(unsigned char output[SHA256_DIGEST_LEN], char *filename)
{
    if (!filename)
        return;
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("file open error!\n");
        return ;
    }
    char buf[1024];
    bzero(buf, 1024);
    MG_SHA256_CTX mg_sha256_ctx;
    mg_sha256_init(&mg_sha256_ctx);
    while(fgets(buf, 1024, fp) != NULL)
    {
        mg_sha256_update(&mg_sha256_ctx, buf, strlen(buf));
        bzero(buf, 1024);
    }
    mg_sha256_final(output, &mg_sha256_ctx);
    fclose(fp);
    return ;
}

