#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mgmurmurhash.h"
#include "mgtime.h"

int main(int argc, char** argv)
{

    char buf[1024];
    uint32_t x86_32_result;
    uint32_t x86_128_result[4];
    uint64_t x64_128_result[2];

    uint64_t t1, t2;
    FILE* file = fopen(argv[1], "r");
    int pattern = atoi(argv[2]);

    printf("filename = %s, pattern = %d\n", argv[1], pattern);

    t1 = mg_get_tick_time_ms();
    while(fgets(buf, 1024, file) != NULL)
    {
        switch(pattern)
        {
            case 1:
                printf("buf: %s\n%0x\n",buf,  mgMurmurhash64(buf, strlen(buf), 16));
                break;
            case 2:
                mgMurmurhash32(buf, strlen(buf), 16);
                break;
            case 3:
                mgMurmurhash3_x86_32(buf, strlen(buf), 16, &x86_32_result);
                break;
            case 4:
                mgMurmurhash3_x86_128(buf, strlen(buf), 16, x86_128_result);
                break;
            case 5:
                mgMurmurhash3_x64_128(buf, strlen(buf), 16, x64_128_result);
        }
    }
    t2 = mg_get_tick_time_ms();
    fclose(file);
    printf("t2-t1 = %d\n", t2-t1);
    return 0;
}
