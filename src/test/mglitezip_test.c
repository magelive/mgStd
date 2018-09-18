 #include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include "mglite_zip.h"
#include "mglite_unzip.h"

void print_hex(u_int8_t *data, u_int64_t len)
{
    printf("data len = %d\n", len);
    u_int64_t i = 0;
    printf("===============================================================================\n");
    for (i = 0; i < len; i++)
    {
        if ( i % 8 == 0)
        {
            printf("HEX :[%04d]\t", i);
        }

        printf("0x%02x\t", data[i]);

        if ( (i+1) % 8 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("===============================================================================\n");
}

#if 0
void print_data(u_int8_t *data, u_int64_t len)
{                                       
    printf("data len = %d\n", len);
    u_int32_t i = 0, f = 0;
    printf("===============================================================================\n");
    for (i = 0; i < len; i++)
    {
        if ( i % 8 == 0)
        {
            if (f == 0)
                printf("HEX :[%04d]\t", i);
            else if (f == 1)
                printf("CHAR:[%04d]\t", i);
            else if ( f == 2 ) 
                printf("----:[----]\t");

        }
        
        if (f== 0)
            printf("0x%02x\t", data[i]);
        else if (f == 1)
            printf("  %c\t", data[i]);
        else if (f == 2)
            printf("----\t");

        if ( (i+1) % 8 == 0)
        {
            printf("\n");
            if (f != 2)
                i -= 8;
            f = (f+1)>2?0:f+1;
        }
    }
    printf("\n");
    
    for (i = (len/8*8); i < len; i++)
    {
        if ( i % 8 == 0)
            printf("CHAR:[%04d]\t", i);
        printf("  %c\t", data[i]);
        if (i == len-1)
            printf("\n");
    }
    printf("===============================================================================\n");
}
#endif

#define print_data print_hex
u_int8_t *zip_memory_data(u_int8_t *data, u_int64_t len, u_int64_t *zip_len)
{
    HZIP hz;
    HANDLE base;
    u_int8_t *pret = NULL;

    //压缩 
    void *out_buff= NULL;
    u_int64_t out_len = 0;
    int nres = 0;

    nres = ZipCreateBuffer(&hz, 0, len+1, 0 );
    if ( nres != ZR_OK ) goto zip_exit_point;

    nres = ZipAddBufferRaw(hz, data, len );
    if ( nres != ZR_OK ) goto zip_exit_point;

    nres = ZipGetMemory( hz, &out_buff, &out_len, &base );
    if ( nres != ZR_OK ) goto zip_exit_point;

    //printf( "压缩后数据：[%s] len:%d\r\n", out_buff, out_len);
    out_len += 4; // 这个坑太深了，尾部还有4个字节，文档都没有说明啊。 

    //---- 加上长度
    pret = (char *)calloc( out_len+4, 1 );
    *( (u_int32_t*) pret ) = len;
    memcpy( pret+4, out_buff, out_len );
    *zip_len = out_len;

zip_exit_point:
    if ( out_buff )
        free(out_buff);

    return pret;
}

u_int8_t *unzip_memory_data(u_int8_t *data, u_int64_t len, u_int64_t *unzip_len)
{
    u_int8_t *pret = NULL;
    u_int32_t iLen = *((u_int32_t*)data);
    HUNZIP huz = NULL;
    int nret = 0;
    printf( "iLen:%d,data_len:%d\r\n", iLen, len );
    print_data( data, len );
    nret = UnzipOpenBufferRaw(&huz, data+ 4, len, NULL);
    if ( ZR_OK == nret )
    {
        ZIPENTRY ze;
        char *zipBuf = NULL;

        ze.CompressedSize = len;
        ze.UncompressedSize = iLen;

        zipBuf = (char*)calloc(1, ze.UncompressedSize);
        if (zipBuf)
        {
            nret = UnzipItemToBuffer(huz,zipBuf,ze.UncompressedSize,&ze);
            if ( ZR_OK == nret )
            {
                //fprintf(stdout, "解压数据:[%s]\r\n", zipBuf );
                pret = strdup( zipBuf );
                *unzip_len = iLen;
            }
            else
            {
                char msg[100];
                UnzipFormatMessage(nret, msg, sizeof(msg));
                printf( "error msg %s\r\n", msg );
            }
            free(zipBuf);
        }
        UnzipClose(huz);
    }
    else
    {
        char msg[100];
        UnzipFormatMessage(nret, msg, sizeof(msg));
        printf( "error msg %s\r\n", msg );
    }
    return pret;
}

void test_zipbuff(char *data, int len)
{
    print_data(data, len);
    u_int64_t zip_len = 0;
    u_int8_t *zip_data = zip_memory_data((u_int8_t *)data, (u_int64_t)len, &zip_len);
    print_data(zip_data, zip_len);
    
    u_int64_t unzip_len = 0;
    u_int8_t *unzip_data = unzip_memory_data(zip_data, zip_len, &unzip_len);
    print_data(unzip_data, unzip_len);
}

int main(int argc, char *argv[])
{
    test_zipbuff(argv[1], strlen(argv[1]));
    return 0;
}
