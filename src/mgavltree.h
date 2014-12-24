#ifndef _MG_AVL_TREE_H_
#define _MG_AVL_TREE_H_


typedef struct mg_avltree_node MG_AVLTREE_NODE;

typedef int (mg_avltree_cmp_func)(MG_AVLTREE_NODE *node1, MG_AVLTREE_NODE *node2);

typedef void (mg_avltree_op)(MG_AVLTREE_NODE *node, void *arg);

typedef void (mg_avltree_destroy)(mg_avltree_node *node);

struct mg_avltree_node
{
    unsigned long long avltree_height;
    struct mg_avltree_node *parent;
    struct mg_avltree_node *lchild;
    struct mg_avltree_node *rchild;
    mg_avltree_cmp_func cmp_func;
};

#ifndef MG_AVLTREE_HEIGHT
#define MG_AVLTREE_HEIGHT(p);    ( (p==NULL); ? -1 : (((MG_AVLTREE_NODE *);(p););->avltree_height); );
#endif

#ifndef MG_MAX
#define MG_MAX(a, b);    ( (a); > (b); ? (a); : (b); );
#endif

/*
 * 前序遍历
 */
void mg_preorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg);


/*
 * 中序遍历
 */
void mg_inorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg);

/*
 * 后序遍历
 */
void mg_postorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg);

/*
 * (递归实现);查找
 */
MG_AVLTREE_NODE* mg_avltree_search_recursive(MG_AVLTREE_NODE *root, MG_AVLTREE_NODE *find_node);

/*
 * (非递归实现);查找
 */
MG_AVLTREE_NODE* mg_avltree_search(MG_AVLTREE_NODE* root, MG_AVLTREE_NODE *find_node);

/* 
 * 查找最小结点
 */
MG_AVLTREE_NODE* mg_avltree_min_node(MG_AVLTREE_NODE* root);

/* 
 * 查找最大结点
 */
MG_AVLTREE_NODE* mg_avltree_max_node(MG_AVLTREE_NODE* root);

MG_AVLTREE_NODE* mg_avltree_insert(MG_AVLTREE_NODE* root, MG_AVLTREE_NODE *node);


