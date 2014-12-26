#ifndef _MG_SPLAY_TREE_H_
#define _MG_SPLAY_TREE_H_

typedef struct mg_splaytree_struct mg_splaytree_t;

typedef int (mg_splaytree_node_cmp_func)(mg_splaytree_t *node1, mg_splaytree_t *node2);

typedef void (mg_splaytree_op)(mg_splaytree_t *node, void *arg);

typedef void (mg_splaytree_node_free)(mg_splaytree_t *node);

struct mg_splaytree_struct
{
    struct mg_splaytree_struct *parent;
    struct mg_splaytree_struct *lchild;
    struct mg_splaytree_struct *rchild;
    mg_splaytree_node_cmp_func *cmp_func;
};

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);\
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif
#define mgsplaytree_entry(ptr, type, member) container_of(ptr, type, member)


void mg_splaytree_node_init(mg_splaytree_t *node, mg_splaytree_node_cmp_func *cmp_func);

//中序遍历 MLR
void mg_preorder_splaytree(mg_splaytree_t *root, mg_splaytree_op opfunc, void *arg);

//前序遍历  LMR
void mg_inorder_splaytree(mg_splaytree_t *root, mg_splaytree_op opfunc, void *arg);

//后序遍历 LRM
void mg_postorder_splaytree(mg_splaytree_t *root, mg_splaytree_op opfunc, void *arg);

//查找最小结点
mg_splaytree_t* mg_splaytree_min_node(mg_splaytree_t* root);

//查找最大结点
mg_splaytree_t* mg_splaytree_max_node(mg_splaytree_t* root);

mg_splaytree_t* mg_splaytree_find(mg_splaytree_t *root, mg_splaytree_t *node);

mg_splaytree_t* mg_splaytree_insert(mg_splaytree_t* root, mg_splaytree_t *node);

mg_splaytree_t *mg_splaytree_remove(mg_splaytree_t *root, mg_splaytree_t *node, mg_splaytree_node_free nfree);

void mg_splaytree_destory(mg_splaytree_t *root, mg_splaytree_node_free nfree);

#endif
