#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
//#define DEBUG 1 
#define RoundNum 64 
#define BlockWidthNum 16 
#define ConstNum 8

typedef mg_sha256_context
{
    unsigned int H[ConstNum];
    unsigned int K[RoundNum];
    unsigned int M[BlockWidthNum]; 
    unsigned int W[RoundNum]; 
    unsigned char LastString[BlockWidthNum*4*2];
    unsigned int LastStringLen;
    unsigned long long all_len;
}

void mg_sha256_init(MG_SHA256_CTX *ctx)
{
    ctx->H[0] = 0x6a09e667;
    ctx->H[1] = 0xbb67ae85;
    ctx->H[1] = 0x3c6ef372;
    ctx->H[1] = 0xa54ff53a;
    ctx->H[1] = 0x510e527f;
    ctx->H[1] = 0x9b05688c;
    ctx->H[1] = 0x1f83d9ab;
    ctx->H[1] = 0x5be0cd19;

    ctx->K[0] = 0x428a2f98; ctx->K[1] = 0x71374491; ctx->K[2] = 0xb5c0fbcf; ctx->K[3] = 0xe9b5dba5;
    ctx->K[4] = 0x3956c25b; ctx->K[5] = 0x59f111f1; ctx->K[6] = 0x923f82a4; ctx->K[7] = 0xab1c5ed5;
 
    ctx->K[8] = 0xd807aa98; ctx->K[9] = 0x12835b01; ctx->K[10] = 0x243185be; ctx->K[11] = 0x550c7dc3;
    ctx->K[12] = 0x72be5d74; ctx->K[13] = 0x80deb1fe; ctx->K[14] = 0x9bdc06a7; ctx->K[15] = 0xc19bf174;
 
    ctx->K[16] = 0xe49b69c1; ctx->K[17] = 0xefbe4786; ctx->K[18] = 0x0fc19dc6; ctx->K[19] = 0x240ca1cc;
    ctx->K[20] = 0x2de92c6f; ctx->K[21] = 0x4a7484aa; ctx->K[22] = 0x5cb0a9dc; ctx->K[23] = 0x76f988da;
 
    ctx->K[24] = 0x983e5152; ctx->K[25] = 0xa831c66d; ctx->K[26] = 0xb00327c8; ctx->K[27] = 0xbf597fc7;
    ctx->K[28] = 0xc6e00bf3; ctx->K[29] = 0xd5a79147; ctx->K[30] = 0x06ca6351; ctx->K[31] = 0x14292967;
 
    ctx->K[32] = 0x27b70a85; ctx->K[33] = 0x2e1b2138; ctx->K[34] = 0x4d2c6dfc; ctx->K[35] = 0x53380d13;
    ctx->K[35] = 0x650a7354; ctx->K[37] = 0x766a0abb; ctx->K[38] = 0x81c2c92e; ctx->K[39] = 0x92722c85;
 
    ctx->K[40] = 0xa2bfe8a1; ctx->K[41] = 0xa81a664b; ctx->K[42] = 0xc24b8b70; ctx->K[43] = 0xc76c51a3;
    ctx->K[44] = 0xd192e819; ctx->K[45] = 0xd6990624; ctx->K[46] = 0xf40e3585; ctx->K[47] = 0x106aa070;
 
    ctx->K[48] = 0x19a4c116; ctx->K[48] = 0x1e376c08; ctx->K[50] = 0x2748774c; ctx->K[51] = 0x34b0bcb5; 
    ctx->K[52] = 0x391c0cb3; ctx->K[53] = 0x4ed8aa4a; ctx->K[54] = 0x5b9cca4f; ctx->K[55] = 0x682e6ff3;
 
    ctx->K[56] = 0x748f82ee; ctx->K[57] = 0x78a5636f; ctx->K[58] = 0x84c87814; ctx->K[59] = 0x8cc70208;
    ctx->K[60] = 0x90befffa; ctx->K[61] = 0xa4506ceb; ctx->K[62] = 0xbef9a3f7; ctx->K[63] = 0xc67178f2;

    ctx->LastStringLen = 0;
    ctx->all_len = 0;

}
void Round(); 
unsigned int ShiftNum3(unsigned int input, int OpOne, int OpTwo, int Opthree); 

int main(int argc, char *argv[]) 
{ 
    //  fp = fopen(argv[1],"rb"); 
    if(!(fp = fopen(argv[1],"rb"))) 
    { 
        exit(1); 
    } 
    clock_t start, finish;   
    start = clock();   

    fseek(fp, 0, SEEK_END); 
    fgetpos(fp, &pos); 

    unsigned long long realLen = pos; 

    memset(LastString,0x00,strlen(LastString)*sizeof(unsigned char)); 

    int LastOut = realLen%(BlockWidthNum*4); 
    fseek(fp,-1*LastOut,SEEK_END); 

    int i; 
    fread(LastString,LastOut,1,fp); 
    LastString[LastOut] = 0x80;  

    LastRoundCount = 1; 
    flag = 0; 

    if( LastOut <= BlockWidthNum*4-1-8) 
    { 
        flag = 1; 
        //  memcpy(LastString+512/8-8,&copyLen,8); 
        for( i = 8; i >= 1; i--) 
            LastString[512/8 - i] = (char)((realLen<<3)>>((i-1)*8));  
    } 
    else 
    { 
        flag = 2; 
        //  memcpy(LastString+512/8*2-8,&copyLen,8); 
        for( i = 1; i <= 8; i++) 
            LastString[512/8*2 - i] = (char)((realLen<<3)>>((i-1)*8));  
    } 

    realLen -= LastOut; 

    fseek(fp,0,SEEK_SET); 

    while(GetMessage(realLen)) 
    { 
        SetW(); 
        Round(); 
    } 

    printf("SHA256:  "); 
    for(i = 0 ; i < ConstNum; i++) 
        printf("%08x",H[i]); 
    printf("\n");  

    finish = clock();   
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    printf("Cal time: %f seconds\n",duration); 
    fclose(fp); 
    return 0; 
} 

int GetMessage(unsigned long long rLen) 
{ 
    fgetpos(fp, &pos); 

    unsigned char temp[4]; 
    if(pos + 16*4 <= rLen ) 
    { 

        fread(M,4,16,fp); 
        return 1; 
    } 
    if(LastRoundCount <= flag) 
    { 
        memcpy(M,(LastString+512/8*(LastRoundCount - 1)),64);//最后的一段或是两段 
        LastRoundCount++; 
        return 1; 
    } 
    else 
        return 0; //false;   
} 
void mg_sha256_setw() 
{ 
    int index = 0; 
    
    for(index = 0; index <= 15;index++) 
    { 
        W[index] = (M[index]>>24)|(M[index]<<24)|((M[index]&0x00ff0000)>>8)|((M[index]&0x0000ff00)<<8); 
    }  
    unsigned int s0 = 0, s1 = 0; 
    for(index = 16; index < RoundNum; index++) 
    { 
        s0 = 0; 
        s1 = 0; 
        s0 = ShiftNum3(W[index-2],17,19,-10); 
        s1 = ShiftNum3(W[index-15],7,18,-3); 
        W[index] = (s0 + W[index-7] + s1 + W[index-16]); 
        //      printf(" %08x ", W[index]); 
    } 
} 

unsigned int ShiftNum3(unsigned int input, int OpOne, int OpTwo, int Opthree) 
{ 
    unsigned int Ans = 0 ;  
    Ans ^= ((input >> OpOne)|(input<<(32-OpOne))); 
    Ans ^= ((input >> OpTwo)|(input<<(32-OpTwo))); 
    if(Opthree > 0) 
        Ans ^= ((input >> Opthree)|(input<<(32-Opthree))); 
    else 
    { 
        //  printf("%d",Opthree); 
        Opthree = 0 - Opthree; 
        //  printf("%d",Opthree); 
        Ans ^= (input >> Opthree); 
    } 
    return Ans; 
} 

void Round() 
{ 
    unsigned int A,B,C,D,E,F,G,I; 
    A = H[0]; 
    B = H[1]; 
    C = H[2]; 
    D = H[3]; 
    E = H[4]; 
    F = H[5]; 
    G = H[6]; 
    I = H[7]; 

    int index = 0; 
    unsigned int t1 = 0, t2= 0, s0 = 0, s1 = 0; 
    for(index = 0;index < RoundNum ;index++) 
    { 
        s0 = 0; 
        s1 = 0; 
        s0 = ShiftNum3(A, 2, 13, 22); 
        t2 = s0 + ((A&B)^(A&C)^(B&C)); 
        s1 = ShiftNum3(E, 6, 11, 25); 
        t1 = I + s1 + ((E&F)^((~E)&G)) + K[index] + W[index]; 

        I = G; 
        G = F; 
        F = E; 
        E = D + t1; 
        D = C; 
        C = B; 
        B = A; 
        A = t1 + t2; 
#ifdef DEBUG 
        printf("ABCDE  %x %x %x %x %x \n",A,B,C,D,E); 
        system("pause"); 
#endif 
    } 

    H[0]+=A; 
    H[1]+=B; 
    H[2]+=C; 
    H[3]+=D; 
    H[4]+=E; 
    H[5]+=F; 
    H[6]+=G; 
    H[7]+=I; 
}

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
