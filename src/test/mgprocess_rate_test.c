#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mgprocess_rate.h"
void main()
{
    mgprocess_t *p1 = mgprocess_init(100, '#', "test1");
    mgprocess_t *p2 = mgprocess_init(20, '*', "test2");
    int i = 0;
    for ( i = 0; i < 101; i++)
    {
        mgprocess_out_rate(p1, i);
        usleep(100000);
    }
    for (i = 0; i < 101; i+=10)
    {
        mgprocess_out_rate(p2, i);
        usleep(1000000);
    }
    mgprocess_finish(p1);
    mgprocess_finish(p2);
    printf("over!\n");
    return;
}

