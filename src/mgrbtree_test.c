#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mgrbtree.h"
//#include "mgavltree.h"

typedef struct mgrbtree_test
{
    int data;
    mgrbtree_t node;
}mrt;

int mrt_cmp(mgrbtree_t *node1, mgrbtree_t *node2)
{
    mrt *mrt1 = mgrbtree_entry(node1, mrt, node);
    mrt *mrt2 = mgrbtree_entry(node2, mrt, node);
    return mrt1->data - mrt2->data;
}

void mrt_op(mgrbtree_t *node, void *arg)
{
    mrt *mrt1 = mgrbtree_entry(node, mrt, node);
    mrt *parent = NULL;
    mrt *lchild = NULL;
    mrt *rchild = NULL;
    printf("data: %d ,", mrt1->data);
    if (node->color == mg_red)
    {
        printf("color:red |");
    }
    else
    {
        printf("color:black |");
    }
    printf("parent: ");
    if (node->parent)
    {
        parent = mgrbtree_entry(node->parent, mrt, node);
        printf("%d color %s |", parent->data, (node->parent->color == mg_red) ? "red" : "black");
    }
    else
    {
        printf("NULL |");
    }
    printf("lchild: ");
    if (node->lchild)
    {
        lchild = mgrbtree_entry(node->lchild, mrt, node);
        printf("%d color %s |", lchild->data, (node->lchild->color == mg_red) ? "red" : "black");
    }
    else
    {
        printf("NULL |");
    }
    printf("rchild: ");
    if (node->rchild)
    {
        rchild = mgrbtree_entry(node->rchild, mrt, node);
        printf(" %d color %s\n", rchild->data, (node->rchild->color == mg_red) ? "red" : "black");
    }
    else
    {
        printf("NULL\n");
    }
    return;
}

int main(int argc, char *argv[])
{
    int a[10] = {1,2,3,5,7,8,6,4,10,9};//5,6,7,8,9, 10};
    mrt m[10];
    mrt del_mrt;
    int i;
    mgrbtree_t *root = NULL;
    for (i = 0; i < 10; i ++)
    {
        m[i].data = a[i];
        mgrbtree_init(&(m[i].node), &mrt_cmp);
        root = mgrbtree_insert(root, &m[i].node);
        mgrbtree_preorder(root, mrt_op, NULL);
        printf("\n\n");
    }
    
    del_mrt.data = 1;
    mgrbtree_init(&del_mrt.node, &mrt_cmp);
    root = mgrbtree_remove(root, &del_mrt.node);
    printf("\n~~~~~~~~remove~%d~~~~~~\n", del_mrt.data);
    mgrbtree_preorder(root, mrt_op, NULL);
 
    del_mrt.data = 4;
    mgrbtree_init(&del_mrt.node, &mrt_cmp);
    root = mgrbtree_remove(root, &del_mrt.node);
    printf("\n~~~~~~~~remove~%d~~~~~~\n", del_mrt.data);
    mgrbtree_preorder(root, mrt_op, NULL);
    
    del_mrt.data = 3;
    mgrbtree_init(&del_mrt.node, &mrt_cmp);
    root = mgrbtree_remove(root, &del_mrt.node);
    printf("\n~~~~~~~~remove~%d~~~~~~\n", del_mrt.data);
    mgrbtree_preorder(root, mrt_op, NULL);
    
    del_mrt.data = 2;
    mgrbtree_init(&del_mrt.node, &mrt_cmp);
    root = mgrbtree_remove(root, &del_mrt.node);
    printf("\n~~~~~~~~remove~%d~~~~~~\n", del_mrt.data);
    mgrbtree_preorder(root, mrt_op, NULL);
    return 0;

}
