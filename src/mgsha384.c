#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "mgsha384.h"

void mg_sha384_init(MG_SHA384_CTX *ctx)
{
    ctx->H[0] = 0xcbbb9d5dc1059ed8; ctx->H[1] = 0x629a292a367cd507;
    ctx->H[2] = 0x9159015a3070dd17; ctx->H[3] = 0x152fecd8f70e5939; 
    ctx->H[4] = 0x67332667ffc00b31; ctx->H[5] = 0x8eb44a8768581511;
    ctx->H[6] = 0xdb0c2e0d64f98fa7; ctx->H[7] = 0x47b5481dbefa4fa4; 

    ctx->K[0] = 0x428a2f98d728ae22; ctx->K[1] = 0x7137449123ef65cd;
    ctx->K[2] = 0xb5c0fbcfec4d3b2f; ctx->K[3] = 0xe9b5dba58189dbbc;  
    ctx->K[4] = 0x3956c25bf348b538; ctx->K[5] = 0x59f111f1b605d019;
    ctx->K[6] = 0x923f82a4af194f9b; ctx->K[7] = 0xab1c5ed5da6d8118;  
    ctx->K[8] = 0xd807aa98a3030242; ctx->K[9] = 0x12835b0145706fbe;
    ctx->K[10] = 0x243185be4ee4b28c; ctx->K[11] = 0x550c7dc3d5ffb4e2;  
    ctx->K[12] = 0x72be5d74f27b896f; ctx->K[13] = 0x80deb1fe3b1696b1;
    ctx->K[14] = 0x9bdc06a725c71235; ctx->K[15] = 0xc19bf174cf692694;  
    ctx->K[16] = 0xe49b69c19ef14ad2; ctx->K[17] = 0xefbe4786384f25e3;
    ctx->K[18] = 0x0fc19dc68b8cd5b5; ctx->K[19] = 0x240ca1cc77ac9c65;  
    ctx->K[20] = 0x2de92c6f592b0275; ctx->K[21] = 0x4a7484aa6ea6e483;
    ctx->K[22] = 0x5cb0a9dcbd41fbd4; ctx->K[23] = 0x76f988da831153b5;  
    ctx->K[24] = 0x983e5152ee66dfab; ctx->K[25] = 0xa831c66d2db43210;
    ctx->K[26] = 0xb00327c898fb213f; ctx->K[27] = 0xbf597fc7beef0ee4;  
    ctx->K[28] = 0xc6e00bf33da88fc2; ctx->K[29] = 0xd5a79147930aa725;
    ctx->K[30] = 0x06ca6351e003826f; ctx->K[31] = 0x142929670a0e6e70;  
    ctx->K[32] = 0x27b70a8546d22ffc; ctx->K[33] = 0x2e1b21385c26c926;
    ctx->K[34] = 0x4d2c6dfc5ac42aed; ctx->K[35] = 0x53380d139d95b3df;  
    ctx->K[36] = 0x650a73548baf63de; ctx->K[37] = 0x766a0abb3c77b2a8;
    ctx->K[38] = 0x81c2c92e47edaee6; ctx->K[39] = 0x92722c851482353b;  
    ctx->K[40] = 0xa2bfe8a14cf10364; ctx->K[41] = 0xa81a664bbc423001;
    ctx->K[42] = 0xc24b8b70d0f89791; ctx->K[43] = 0xc76c51a30654be30;  
    ctx->K[44] = 0xd192e819d6ef5218; ctx->K[45] = 0xd69906245565a910;
    ctx->K[46] = 0xf40e35855771202a; ctx->K[47] = 0x106aa07032bbd1b8;  
    ctx->K[48] = 0x19a4c116b8d2d0c8; ctx->K[49] = 0x1e376c085141ab53;
    ctx->K[50] = 0x2748774cdf8eeb99; ctx->K[51] = 0x34b0bcb5e19b48a8;  
    ctx->K[52] = 0x391c0cb3c5c95a63; ctx->K[53] = 0x4ed8aa4ae3418acb;
    ctx->K[54] = 0x5b9cca4f7763e373; ctx->K[55] = 0x682e6ff3d6b2b8a3;  
    ctx->K[56] = 0x748f82ee5defb2fc; ctx->K[57] = 0x78a5636f43172f60;
    ctx->K[58] = 0x84c87814a1f0ab72; ctx->K[59] = 0x8cc702081a6439ec;  
    ctx->K[60] = 0x90befffa23631e28; ctx->K[61] = 0xa4506cebde82bde9;
    ctx->K[62] = 0xbef9a3f7b2c67915; ctx->K[63] = 0xc67178f2e372532b;  
    ctx->K[64] = 0xca273eceea26619c; ctx->K[65] = 0xd186b8c721c0c207;
    ctx->K[66] = 0xeada7dd6cde0eb1e; ctx->K[67] = 0xf57d4f7fee6ed178;  
    ctx->K[68] = 0x06f067aa72176fba; ctx->K[69] = 0x0a637dc5a2c898a6;
    ctx->K[70] = 0x113f9804bef90dae; ctx->K[71] = 0x1b710b35131c471b;  
    ctx->K[72] = 0x28db77f523047d84; ctx->K[73] = 0x32caab7b40c72493;
    ctx->K[74] = 0x3c9ebe0a15c9bebc; ctx->K[75] = 0x431d67c49c100d4c;
    ctx->K[76] = 0x4cc5d4becb3e42b6; ctx->K[77] = 0x597f299cfc657e2a;
    ctx->K[78] = 0x5fcb6fab3ad6faec; ctx->K[79] = 0x6c44198c4a475817;

    bzero(ctx->LastString, sizeof(ctx->LastString));
    ctx->LastStringLen = 0;
    ctx->all_len = 0;
}

unsigned long long mg_sha384_shift_num3(unsigned long long input, int OpOne, int OpTwo, int Opthree) 
{ 
    unsigned long long Ans = 0 ;  
    Ans ^= ((input >> OpOne)|(input<<(64-OpOne))); 
    Ans ^= ((input >> OpTwo)|(input<<(64-OpTwo))); 
    if(Opthree > 0) 
        Ans ^= ((input >> Opthree)|(input<<(64-Opthree))); 
    else 
    { 
        Opthree = 0 - Opthree; 
        Ans ^= (input >> Opthree); 
    } 
    return Ans; 
} 

void mg_sha384_setw(MG_SHA384_CTX *ctx) 
{ 
    int index = 0; 

    for(index = 0; index <= 15;index++) 
    { 
        ctx->W[index] = (ctx->M[index]>>56) | (ctx->M[index]<<56) | 
                    ((ctx->M[index]&0x00ff000000000000)>>40) | ((ctx->M[index]&0x000000000000ff00)<<40) | 
                    ((ctx->M[index]&0x0000ff0000000000)>>24) | ((ctx->M[index]&0x0000000000ff0000)<<24) | 
                    ((ctx->M[index]&0x000000ff00000000)>>8)  | ((ctx->M[index]&0x00000000ff000000)<<8); 
    }  
    unsigned long long s0 = 0, s1 = 0; 
    for(index = 16; index < SHA384_ROUNDNUM; index++) 
    { 
        s0 = 0; 
        s1 = 0; 
        s0 = mg_sha384_shift_num3(ctx->W[index-2],19,61,-6); 
        s1 = mg_sha384_shift_num3(ctx->W[index-15],1,8,-7); 
        ctx->W[index] = (s0 + ctx->W[index-7] + s1 + ctx->W[index-16]); 
    } 
} 

void mg_sha384_round(MG_SHA384_CTX *ctx) 
{ 
    unsigned long long A,B,C,D,E,F,G,I; 
    A = ctx->H[0]; 
    B = ctx->H[1]; 
    C = ctx->H[2]; 
    D = ctx->H[3]; 
    E = ctx->H[4]; 
    F = ctx->H[5]; 
    G = ctx->H[6]; 
    I = ctx->H[7]; 

    int index = 0; 
    unsigned long long t1 = 0, t2= 0, s0 = 0, s1 = 0; 
    for(index = 0;index < SHA384_ROUNDNUM ;index++) 
    { 
        s0 = 0; 
        s1 = 0; 
        s0 = mg_sha384_shift_num3(A, 28, 34, 39); 
        t2 = s0 + ((A&B)^(A&C)^(B&C)); 
        s1 = mg_sha384_shift_num3(E, 14, 18, 41); 
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

void mg_sha384_update(MG_SHA384_CTX *ctx, char *input, int input_len)
{
    int i, process_len = 0;
    ctx->all_len += input_len;
    if (input_len+ctx->LastStringLen < 16 * 8)
    {
        memcpy(ctx->LastString+ctx->LastStringLen, input, input_len);
        ctx->LastStringLen += input_len;
        return;
    }
    else
    {
        process_len = 0;
        for (i = 0; i < ctx->LastStringLen/128; i++)
        {
            bzero(ctx->M, sizeof(ctx->M));
            memcpy(ctx->M, ctx->LastString+process_len, 128);
            process_len += 128;
            mg_sha384_setw(ctx);
            mg_sha384_round(ctx);

        }
        bzero(ctx->M, sizeof(ctx->M));
        memcpy(ctx->M, ctx->LastString + process_len, ctx->LastStringLen%128);
        memcpy(((char *)ctx->M)+ctx->LastStringLen%128, input, 128-(ctx->LastStringLen%128));
        input_len -= (128-(ctx->LastStringLen%128));
        process_len = 128 - (ctx->LastStringLen%128);
        mg_sha384_setw(ctx);
        mg_sha384_round(ctx);
    }

    for(i = 0; i < input_len/128; i++)
    {
        memcpy(ctx->M, input+process_len, 128);
        process_len += 128;
        mg_sha384_setw(ctx);
        mg_sha384_round(ctx);
    }
    bzero(ctx->LastString, sizeof(ctx->LastString));
    memcpy(ctx->LastString, input+process_len, input_len%128);
    ctx->LastStringLen = input_len%128;
    return;
}

void mg_sha384_final(unsigned char output[SHA384_DIGEST_LEN], MG_SHA384_CTX *ctx)
{
    unsigned long long realLen = ctx->all_len;
    int flag=0, LastRoundCount = 1;
    int i;
    ctx->LastString[realLen%(SHA384_BLOCKWIDTHNUM*8)] = 0x80;
    if(ctx->LastStringLen <= SHA384_BLOCKWIDTHNUM*8-1-16)  
    {
        flag = 1;
        for( i = 8; i >= 1; i--) 
            ctx->LastString[SHA384_BLOCKWIDTHNUM*8 - i] = (char)((realLen<<3)>>((i-1)*8));   
    } 
    else 
    { 
        flag = 2;
        for( i = 1; i <= 8; i++) 
            ctx->LastString[SHA384_BLOCKWIDTHNUM*8*2 - i] = (char)((realLen<<3)>>((i-1)*8));   
    }
    while(LastRoundCount <= flag)
    {
        memcpy(ctx->M, (ctx->LastString+SHA384_BLOCKWIDTHNUM*8*(LastRoundCount - 1)),
                SHA384_BLOCKWIDTHNUM*8);//最后的一段或两段 
        LastRoundCount++;
        mg_sha384_setw(ctx);
        mg_sha384_round(ctx);
    }
    for(i = 0; i < SHA384_CONSTNUM-2; i++)
    {
        sprintf(output+i*16, "%016llx", ctx->H[i]);
    }
    return; 
}

void mg_sha384_calc(unsigned char output[SHA384_DIGEST_LEN], char *buf, int buf_len)
{
    MG_SHA384_CTX mg_sha384_ctx;
    mg_sha384_init(&mg_sha384_ctx);
    mg_sha384_update(&mg_sha384_ctx, buf, buf_len);
    mg_sha384_final(output, &mg_sha384_ctx);
}

void mg_sha384_file(unsigned char output[SHA384_DIGEST_LEN], char *filename)
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
    MG_SHA384_CTX mg_sha384_ctx;
    mg_sha384_init(&mg_sha384_ctx);
    while(fgets(buf, 1024, fp) != NULL)
    {
        mg_sha384_update(&mg_sha384_ctx, buf, strlen(buf));
        bzero(buf, 1024);
    }
    mg_sha384_final(output, &mg_sha384_ctx);
    fclose(fp);
    return ;
}


