#include <stdio.h>
#include <stdlib.h>

#include "mgavltree.h"

typedef struct mg_avltree_test
{
    int data;
    MG_AVLTREE_NODE node;
}mat_t;

int mat_cmp_func(MG_AVLTREE_NODE *node1, MG_AVLTREE_NODE *node2)
{
   mat_t *mat1 =  mgavltree_entry(node1, mat_t, node);
   mat_t *mat2 =  mgavltree_entry(node2, mat_t, node);

   return (mat1->data - mat2->data);
   
}

void mat_print(MG_AVLTREE_NODE *node, void *arg)
{
    mat_t *mat = mgavltree_entry(node, mat_t, node);
    printf("data = %d\n", mat->data);
}


int main(int argc, char *argv[])
{
    //3 2 1 4 5 6 7 16 15 14 13 12 11 10 8 9 
    int a[16] = {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};//{1,4,5,9,6,8,7,3,2};
    int i;
    mat_t mat[16];
    MG_AVLTREE_NODE *root = NULL;
    for(i = 0; i< 16; i++)
    {
        mat[i].data = a[i];
        MG_AVLTREE_INIT(&(mat[i].node), &mat_cmp_func);
        root = mg_avltree_insert(root, &(mat[i].node));
    }

    mg_preorder_avltree(root, mat_print, NULL);
    printf("!!!!!!!!\n");
    mg_inorder_avltree(root, mat_print, NULL);
    printf("~~~~~~~~~~~\n");
    mg_postorder_avltree(root, mat_print, NULL);
}
