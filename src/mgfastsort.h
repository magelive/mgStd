#ifndef _MG_FAST_SORT_
#define _MG_FAST_SORT_

//node1>node2 return > 0, node1<node2 return < 0
typedef int (mg_fast_sort_cmp_func)(void *node1, void *node2);

typedef void (mg_fast_sort_swap)(void *list, int i, int j);


/************************
fastsort:
    cmp:  Element comparison function.
    swap: Switching elements i and j in the list is located.
    list: Need a sorted list, and the sort list.
    list_sort_start: Sort starting position.
    list_sort_end: Sort end position.
    type_length: Element length.


  ***********************/
void mgfastsort(mg_fast_sort_cmp_func cmp, mg_fast_sort_swap swap, void *list, int list_sort_start, 
        int list_sort_end, int type_length);
#endif
