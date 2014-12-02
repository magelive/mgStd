#ifndef __MG_MURMURHASH__
#define __MG_MURMURHASH__
/********************************


  ******************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
   64位哈希值
 */
uint64_t mgMurmurhash64 ( const void * key, int len, unsigned int seed );

/*
   32位哈希值
 */
uint32_t mgMurmurhash32 ( const void * key, int len, unsigned int seed );

/*
    out为一个unit32_t 
 */
void mgMurmurhash3_x86_32 ( const void * key, int len, uint32_t seed, void * out );

/*
   out 为四个unit32_t
 */
void mgMurmurhash3_x86_128 ( const void * key, const int len, uint32_t seed, void * out );

/*
   out为两个uinit64_t
 */
void mgMurmurhash3_x64_128 ( const void * key, const int len, const uint32_t seed, void * out );



#endif
