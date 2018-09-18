#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mgbase64.h"


int main(int argc, char *argv[])
{
    unsigned char *str = "Hello, I don't now how to say!\0";
    size_t str_len = 31;//strlen("Hello, I don't now how to say!\0");
    char *result;
    mg_encode_base64(str, str_len, &result);
    printf("encode : %s\n", result);
    unsigned char *dres;
    mg_decode_base64(result, &dres);
    printf("\n\ndecode : %s\n", dres);
    return 0;
}
