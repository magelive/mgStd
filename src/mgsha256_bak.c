#include<stdio.h> 
#include<stdlib.h> 
#include<time.h> 
//#define DEBUG 1 
#define RoundNum 64 
#define BlockWidthNum 16 
#define ConstNum 8

unsigned int H[ConstNum] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19}; 
unsigned int K[RoundNum] = { 
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2}; 

unsigned int M[BlockWidthNum]; 
unsigned int W[RoundNum]; 
unsigned char LastString[BlockWidthNum*4*2]; 
FILE * fp; 
fpos_t pos; //文件长度 
int flag ;  
int LastRoundCount; 

int GetMessage(unsigned long long rLen); 
void SetW(); 
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
void SetW() 
{ 
    int index = 0; 
    //  memcpy(W,M,16*4); 

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
