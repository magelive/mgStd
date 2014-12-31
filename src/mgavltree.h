#ifndef _MG_AVL_TREE_H_
#define _MG_AVL_TREE_H_


typedef struct mg_avltree_node MG_AVLTREE_NODE;

typedef int (mg_avltree_cmp_func)(MG_AVLTREE_NODE *node1, MG_AVLTREE_NODE *node2);

typedef void (mg_avltree_op)(MG_AVLTREE_NODE *node, void *arg);

typedef void (mg_avltree_destroy)(MG_AVLTREE_NODE *node);

struct mg_avltree_node
{
    long long avltree_height;
    struct mg_avltree_node *parent;
    struct mg_avltree_node *lchild;
    struct mg_avltree_node *rchild;
    mg_avltree_cmp_func *cmp_func;
};

#ifndef MG_AVLTREE_HEIGHT
#define MG_AVLTREE_HEIGHT(p)    ( (p == 0x00) ? (-1) : (((MG_AVLTREE_NODE *)(p))->avltree_height) )
#endif

#ifndef MG_MAX
#define MG_MAX(a, b)    ( (a) > (b) ? (a) : (b) )
#endif


#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);\
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif
/*
#define mgsplaytree_entry(ptr, type, member) container_of(ptr, type, member)

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);\
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif
*/
#define mgavltree_entry(ptr, type, member) container_of(ptr, type, member)


void MG_AVLTREE_INIT(MG_AVLTREE_NODE *node, mg_avltree_cmp_func *cmp_func);

/*
 * 前序遍历,MLR
 */
void mg_preorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg);


/*
 * 中序遍历,LMR
 */
void mg_inorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg);

/*
 * 后序遍历,LRM
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

#endif
