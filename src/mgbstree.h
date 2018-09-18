#ifndef __MG_BINARY_SORT_TREE_H_
#define __MG_BINARY_SORT_TREE_H_

typedef struct mg_Binary_sort_tree MG_BS_TREE;

typedef int (mg_BStree_cmp_func)(MG_BS_TREE *node1, MG_BS_TREE *node2);

typedef void (mg_BStree_op_func)(MG_BS_TREE *node, void *argv);

struct mg_Binary_sort_tree
{
    struct mg_Binary_sort_tree *parent;
    struct mg_Binary_sort_tree *rchild;
    struct mg_Binary_sort_tree *lchild;
    mg_BStree_cmp_func *cmp_func;
};


#define INIT_MG_BSTREE_NODE(cmp) {NULL, NULL, NULL, cmp}

#define INIT_MG_BSTREE_ROOT(cmp) \
    struct mg_Binary_sort_tree root = INIT_MG_BSTREE_NODE(cmp)

//#define MG_BSTREE_INIT_NODE(node, cmp) \
//    do{node->parent = NULL;node->rchild = NULL;node->lchild = NULL;node->cmp_func = cmp;}while(0)

#define MG_BSTREE_INSERT_RIGHT(root, right) \
    {root->rchild = right; right->parent = root;}

#define MG_BSTREE_INSERT_LEFT(root, left) \
    {root->lchild = left; left->parent = root;}

#define MG_BS_TREE_IS_ROOT(node)    (node->parent == NULL)

//#define MG_BSTREE_GET_PARENT_CHILD(node) \
//  ((node->parent == NULL)?node:((node == node->parent->lchild)?(node->parent->lchild):(node->parent->rchild)))

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);\
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif 

#define mgBstree_entry(ptr, type, member) container_of(ptr, type, member)

int mg_Bstree_insert(MG_BS_TREE *root, MG_BS_TREE *node);

void mg_Bstree_del(MG_BS_TREE *root, MG_BS_TREE *node);

MG_BS_TREE *mg_Bstree_search(MG_BS_TREE *root, MG_BS_TREE *node);

void mg_Bstree_search_recursive(MG_BS_TREE *root, MG_BS_TREE *node, MG_BS_TREE **result);

#define mg_Bstree_visit mg_Bstree_visit_LMR

void mg_Bstree_visit_LMR(MG_BS_TREE *root, mg_BStree_op_func *op_func, void *arg);
void mg_Bstree_visit_MLR(MG_BS_TREE *root, mg_BStree_op_func *op_func, void *arg);
void mg_Bstree_visit_LRM(MG_BS_TREE *root, mg_BStree_op_func *op_func, void *arg);

void MG_BSTREE_INIT_NODE(MG_BS_TREE *node, mg_BStree_cmp_func *cmp);
#endif 
