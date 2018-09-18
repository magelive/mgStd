#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "mgcrc32.h"

int main(int argc, char *argv[])
{
    unsigned char output[9];
    uint32_t result = 0;
    bzero(output, 9);
    if (argc > 2)
    {
        result = mg_checksum_crc32(output, argv[2], strlen(argv[2]));
        printf(" result =%0x, output = %s, str = %s\n", result, output, argv[2]);
    }
    if (argc > 1)
    {
        result = mg_checksum_crc32_file(output,argv[1]);
        printf(" result =%0x, output = %s, filename = %s\n", result, output, argv[1]);
    }
    return 0;
}

