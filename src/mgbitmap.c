#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mgbitmap.h"

//bit 15, 2^15 = 32,768 (0 ~ 32,767)  	4KB mem
//bit 16, 2^16 = 65,536 (0 ~ 65,535)	8KB mem
//bit 32, 2^32 = 4,294,967,296		524,288 KB mem = 512MB

struct mgbitmap {
	uint16_t last;
	uint16_t min;
	uint16_t max;
	uint16_t map[4096];		//4096*16 = 32768 
};

mgbitmap_t *mgbitmap_init(uint16_t min, uint16_t max)
{
	if (min < 0 || max < 0 || min > max) {
		return NULL;
	}
	
	mgbitmap_t *bm = malloc(sizeof(mgbitmap_t));
	if (!bm) {
		return NULL;
	}
	
	bm->min = min;
	bm->max = max;
	bm->last = bm->min;
	return bm;
}

static int test_and_set_bit(mgbitmap_t *bm,  uint16_t offset)
{
	if (bm->map[offset >> 0x03 - 1] & (1<<(offset & 0x0F))) {
		return 0;
	}

	bm->map[offset >> 0x03 - 1] |= (1<<(offset & 0x0F));
	return 1;
}

uint16_t mgbitmap_alloc(mgbitmap_t *bm)
{
	uint16_t offset = bm->last;
	uint16_t count = 0;
	while (offset) {
		
		if (count++ == (bm->max - bm->min)) {
			return bm->max;
		}
		
		if (++offset >= bm->max) {
			offset = bm->min + 1;
		}
		
		if (test_and_set_bit(bm, offset )) {
			//printf(".........offset = %d.\n", offset);
			break;
		}
	
	}
	bm->last = offset;
	return offset;
}

void mgbitmap_free(mgbitmap_t *bm, uint16_t offset)
{
	bm->map[offset >> 0x03 - 1] &= (~(1<<(offset & 0x0F)));
}

void mgbitmap_finish(mgbitmap_t *bm)
{
	free(bm);
}

int main()
{
	int min = 1;
	int max = 100;
	mgbitmap_t *bm = mgbitmap_init(min, max);
	uint16_t p = 0;
	int i,j;
	for (i = min; i < max; i++) {
		printf("i = %d, free rule is (p mod %d)\n", i, i+1);
		for (j = min; j < max; j++) {
			p = mgbitmap_alloc(bm);
			if (p == max) {
				break;
			} else {
				printf("[%02d %02d] p = %d\n",i, j, p);
			}

			if (p % (i+1) == 0) {
				mgbitmap_free(bm, p);
				//printf("mgbitmap_free p = %d\n", p);
			}
		}
		
		printf("========================\n");

		if (p == max) {
			break;
		}
	}
	return 0;
}
