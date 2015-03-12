#include <stdio.h>
#include <stdlib.h>

#include "mgfastsort.h"


static int mg_pivot(int s, int e)
{
    return ((s+e)/2);
}

void mgfastsort(mg_fast_sort_cmp_func cmp, mg_fast_sort_swap swap, void *list, int list_sort_start, 
        int list_sort_end, int type_lenth)
{
    if (cmp == NULL || swap == NULL || list == NULL 
            || list_sort_start >= list_sort_end)
        return;

    void *key = NULL;
    
    int pivot, i, j;

    pivot = mg_pivot(list_sort_start, list_sort_end);
    swap(list, list_sort_start, pivot);
    key = list+(list_sort_start * type_lenth);
    i = list_sort_start+1;
    j = list_sort_end;
    while(i <= j)
    {
        while((i < list_sort_end) && (cmp(list+i*type_lenth, key) < 0))
        {
            i++;
        }
        while((j > list_sort_start) && (cmp(list+j*type_lenth, key) > 0))
        {
            j--;
        }
        if (i < j)
            swap(list, i, j);
    }
    swap(list, list_sort_start, j);
    mgfastsort(cmp, swap, list, list_sort_start,j-1, type_lenth);
    mgfastsort(cmp, swap, list, j+1,list_sort_end, type_lenth);
    return;
}

