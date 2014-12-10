#include <stdio.h>
#include <stdlib.h>

#include "mgBstree.h"

typedef struct mgBstree_test
{
    int key;
    MG_BS_TREE tree_node;

}MG_BS_TREE_TEST;

int mgBstree_test_cmp(MG_BS_TREE *node1, MG_BS_TREE *node2)
{
    struct mgBstree_test *test1 = mgBstree_entry(node1, MG_BS_TREE_TEST, tree_node);
    struct mgBstree_test *test2 = mgBstree_entry(node2, MG_BS_TREE_TEST, tree_node);
    return (test1->key - test2->key);
}

int tree_count;

void mgBstree_test_op(MG_BS_TREE *node, void *arg)
{
    struct mgBstree_test *test1 = mgBstree_entry(node, MG_BS_TREE_TEST, tree_node);
    printf("key = %d ", test1->key);
    tree_count ++;
    return;
}

int main(int argc, char *argv[])
{
    struct mgBstree_test test[100];
    struct mgBstree_test find_test, del_test;
    MG_BSTREE_INIT_NODE(&find_test.tree_node, &mgBstree_test_cmp);
    int i,j;
    for(i = 0; i < 100; i++)
    {
        test[i].key = i+100;
        MG_BSTREE_INIT_NODE(&test[i].tree_node, &mgBstree_test_cmp);
    }
    
    struct mgBstree_test *root = &test[50];
    for (j = 0; j < 10; j++)
    {
        for (i = 0; i < 10; i++)
        {
            mg_Bstree_insert(&(root->tree_node), &(test[i*10+j].tree_node));
        }
    }
   
    tree_count = 0;
    mg_Bstree_visit(&root->tree_node, mgBstree_test_op, NULL);
    printf("\ntree len = %d\n", tree_count);

    find_test.key = 112;
    MG_BS_TREE *find = NULL;
    
    mg_Bstree_find(&root->tree_node, &find_test.tree_node, &find);
    printf("find = %x\n", find);
    if (find != NULL)
    {
        MG_BS_TREE_TEST *ft = mgBstree_entry(find, MG_BS_TREE_TEST, tree_node);
        printf("ft->key = %d\n", ft->key);
    }
    else
    {
        printf("No this node!\n");
    }

    mg_Bstree_del(&root->tree_node, find);
    
    tree_count = 0;
    mg_Bstree_visit(&root->tree_node, mgBstree_test_op, NULL);
    printf("\ntree len = %d\n", tree_count);

    return 1;
}
