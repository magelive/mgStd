
#ifndef _MG_BITMAP_H
#define _MG_BITMAP_H

typedef struct mgbitmap mgbitmap_t;

mgbitmap_t *mgbitmap_init(uint16_t min, uint16_t max);
uint16_t mgbitmap_alloc(mgbitmap_t *bm);
void mgbitmap_free(mgbitmap_t *bm, uint16_t num);
void mgbitmap_finish(mgbitmap_t *bm);

#endif
