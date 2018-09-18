#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>

#include "mgsha256.h"

int main(int argc, char *argv[])
{
	unsigned char output[64];
	mg_sha256_file(output, argv[1]);
	printf("%s , sha256 : %s\n", output, argv[1]);
	return 0;
}
