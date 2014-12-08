#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>

#include "mgsha384.h"

int main(int argc, char *argv[])
{
	unsigned char output[128];
	mg_sha384_file(output, argv[1]);
	printf("%s , sha384 : %s\n", output, argv[1]);
    return 1;
}
