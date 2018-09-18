#include <stdio.h>
#include <stdlib.h>

#include "mgsplaytree.h"

typedef struct mgsplaytree_test
{
    int data;
    mg_splaytree_t node;
}mstt_t;

int mspt_cmp(mg_splaytree_t *node1, mg_splaytree_t *node2)
{
    mstt_t *mst1 = mgsplaytree_entry(node1, mstt_t, node);
    mstt_t *mst2 = mgsplaytree_entry(node2, mstt_t, node);
    return (mst1->data - mst2->data);
}

void mspt_print(mg_splaytree_t *node, void *arg)
{
    mstt_t *mst = mgsplaytree_entry(node, mstt_t, node);
    printf("data = %d\n", mst->data);
}

void mspt_free(mg_splaytree_t *node)
{
    mstt_t *mst = mgsplaytree_entry(node, mstt_t, node);
    printf("remove data = %d\n", mst->data);
}

int main(int argc, char *argv[])
{
    int i = 0;
    mstt_t mst[6];
    mstt_t mst_find;
    mg_splaytree_t *root = NULL;
    for (i = 0; i < 6; i++)
    {
        mst[i].data = i+1;
        mg_splaytree_node_init(&mst[i].node, &mspt_cmp);
        root = mg_splaytree_insert(root, &mst[i].node);
    }
    mg_preorder_splaytree(root, &mspt_print, NULL);
    printf("\n");

    mst_find.data = 2;
    mg_splaytree_node_init(&mst_find.node, &mspt_cmp);
    root = mg_splaytree_find(root, &mst_find.node);
    mg_preorder_splaytree(root, &mspt_print, NULL);
    printf("\n");
    
    mst_find.data = 4;
    mg_splaytree_node_init(&mst_find.node, &mspt_cmp);
    root = mg_splaytree_find(root, &mst_find.node);
    mg_preorder_splaytree(root, &mspt_print, NULL);
    printf("\n");
    
    mst_find.data = 5;
    mg_splaytree_node_init(&mst_find.node, &mspt_cmp);
    root = mg_splaytree_remove(root, &mst_find.node, &mspt_free);
    mg_preorder_splaytree(root, &mspt_print, NULL);
    
    return 0;
}
