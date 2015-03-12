#include <stdio.h>
#include <stdlib.h>

#include "mgfastsort.h"

//#ifdef _MG_FAST_SORT_TEST_

typedef struct fast_sort_test_node
{
    int key;
    void *data;
}fstn;

int fast_sort_test_cmp(void *node1, void *node2)
{
    fstn *fstn1 = (fstn *)node1;
    fstn *fstn2 = (fstn *)node2;
    return (fstn1->key - fstn2->key);
}

void fast_sort_test_swap(void *list, int i, int j)
{
    fstn *fstn1 = &((fstn *)list)[i];
    fstn *fstn2 = &((fstn *)list)[j];

    fstn *tmp = (fstn *)malloc(sizeof(fstn));
    
    tmp->key = fstn1->key;
    tmp->data = fstn1->data;

    fstn1->key = fstn2->key;
    fstn1->data = fstn2->data;

    fstn2->key = tmp->key;
    fstn2->data = tmp->data;

    free(tmp);
    return;
}

static void print_list(fstn *list, int n)
{
    int i = 0;
    printf("=====fast sort list =====\n");
    for(i = 0; i < n; i++)
    {
        printf("%8d", list[i].key);
        if ((i+1) % 10 == 0) printf("\n");
    }
    if (i%10 != 0) printf("\n");
    printf("=========================\n");
}

int main()
{
    fstn *test_list = (fstn *)malloc(sizeof(fstn) * 10);
    test_list[0].key = 2;
    test_list[1].key = 4;
    test_list[2].key = 5;
    test_list[3].key = 3;
    test_list[4].key = 8;
    test_list[5].key = 6;
    test_list[6].key = 9;
    test_list[7].key = 1;
    test_list[8].key = 7;
    test_list[9].key = 10;

    print_list(test_list, 10);

    mgfastsort(fast_sort_test_cmp, fast_sort_test_swap, (void *)(test_list), 2, 7, sizeof(fstn));

    print_list(test_list, 10);
    return 0;
}
//#endif
