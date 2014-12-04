#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <stdint.h>
#include <string.h>
unsigned int H[5] = {0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0}; 
unsigned int K[4] = {0x5a827999,0x6ed9eba1,0x8f1bbcdc,0xca62c1d6}; 

unsigned int M[16]; 
unsigned int W[80]; 
unsigned char LastString[512/8*2];//将最后多余部分拿出，和补的位存放于此 
unsigned int A,B,C,D,E; 

FILE * fp; 
//fpos_t pos; //文件长度 
uint64_t pos;
int flag ;  
int LastRoundCount; 

int GetMessage(unsigned long long rLen); 
void SetW(); 
void Round(); 

int main(int argc, char *argv[]) 
{ 
    if(argc == 1) 
    { 
        printf("请输入要hash的文件名！\n"); 
        exit(0); 
    } 
    fp = fopen(argv[1],"rb"); 

    clock_t start, finish;  //计时 
    start = clock();   

    fseek(fp, 0, SEEK_END); 
    fgetpos(fp, (fpos_t*)&pos); 
    unsigned long long realLen = pos; 

    memset(LastString,0x00,sizeof(LastString));//strlen(LastString)*sizeof(unsigned char));//初始化 

    int LastOut = realLen%(512/8); 
    fseek(fp,-1*LastOut,SEEK_END); 

    int i; 
    fread(LastString,LastOut,1,fp);//将多余部分复制到LastString 
    LastString[LastOut] = 0x80;  

    LastRoundCount = 1; 
    flag = 0; 
    //  unsigned long long copyLen = realLen<<3; 
    if(LastOut <=55)            //将长度放在末尾 
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

    realLen -= LastOut; //长度变短 

    fseek(fp,0,SEEK_SET); 

    while(GetMessage(realLen)) 
    { 
        SetW(); 
        Round(); 
        //      printf("."); 
    } 

    printf("SHA1:  "); 
    for(i = 0 ; i < 5; i++) 
        printf("%08x ",H[i]); 
    printf("\n");  

    finish = clock();   
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;  
    printf("Cal time: %f seconds\n",duration); 
    fclose(fp); 
    return 0; 
} 

int GetMessage(unsigned long long rLen) 
{ 
    fgetpos(fp, (fpos_t *)&pos); 

    if(pos + 16 * 4 <= rLen) 
    { 
        fread(M,4,16,fp);//注意M的读入，读入之后M的四个字节是反过来的，应该是编码方式的问题，big-edi和little-edi 
        return 1;           //所以后面存到W时要移位操作，此处这样操作，读入速度变快 
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
void SetW() 
{ 
    int index = 0; 
    for(index = 0; index <=15;index++) 
    {   //编码方式，M的四个字节的顺序是反过来的，将其移位 
        W[index] = (M[index]>>24)|(M[index]<<24)|((M[index]&0x00ff0000)>>8)|((M[index]&0x0000ff00)<<8); 
    }  

    for(index = 16;index<=79;index++) 
    { 
        unsigned int temp = W[index -16]^W[index - 14]^W[index - 8]^W[index - 3]; 
        W[index] = (temp<<1)|(temp>>(32 -1)); 
    } 
} 

void Round() 
{ 
    A = H[0]; 
    B = H[1]; 
    C = H[2]; 
    D = H[3]; 
    E = H[4]; 
    int index = 0; 
    unsigned int temp; 
    for(index = 0;index <=79;index++) 
    { 
        if(index >= 0 && index <=19) 
            temp = E + ((B&C)|(~B&D)) + ((A<<5)|(A>>(32-5))) + W[index] + K[0]; 
        else if(index >= 20 && index <=39) 
            temp = E + (B^C^D) + ((A<<5)|(A>>(32-5))) + W[index] + K[1]; 
        else if(index >= 40 && index <=59) 
            temp = E + ((B&C)|(C&D)|(B&D)) + ((A<<5)|(A>>(32-5))) + W[index] + K[2]; 
        else if(index >= 60 && index <=79) 
            temp = E + (B^C^D) + ((A<<5)|(A>>(32-5))) + W[index] + K[3]; 

        E = D; 
        D = C; 
        C = (B<<30)|(B>>(32-30)); 
        B = A; 
        A = temp; 
    } 

    H[0]+=A; 
    H[1]+=B; 
    H[2]+=C; 
    H[3]+=D; 
    H[4]+=E; 
} 

