#ifndef _MG_RBTREE_H_
#define _MG_RBTREE_H_

typedef enum mgrbtree_color
{
    mg_black,
    mg_red
}mgrbtree_color_t;

typedef struct mgrbtree_node mgrbtree_t;

typedef int (mgrbtree_cmp_func)(mgrbtree_t *node1, mgrbtree_t *node2);

typedef void (mg_rbtree_op)(mgrbtree_t *node, void *arg);

struct mgrbtree_node
{
    mgrbtree_color_t        color;
    mgrbtree_t              *parent;
    mgrbtree_t              *lchild;
    mgrbtree_t              *rchild;
    mgrbtree_cmp_func       *cmp_func;
};

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);\
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define mgrbtree_entry(ptr, type, member) container_of(ptr, type, member)

void mgrbtree_init(mgrbtree_t *node, mgrbtree_cmp_func *cmp_func);

void mgrbtree_preorder(mgrbtree_t *root, mg_rbtree_op opfunc, void *arg);

mgrbtree_t *mgrbtree_insert(mgrbtree_t *root, mgrbtree_t *node);

mgrbtree_t *mgrbtree_search(mgrbtree_t *root, mgrbtree_t *node);

mgrbtree_t *mgrbtree_remove(mgrbtree_t *root, mgrbtree_t *node);

mgrbtree_t *mgrbtree_remove_node(mgrbtree_t *root, mgrbtree_t *node);

mgrbtree_t *mgrbtree_min_node(mgrbtree_t *root);

mgrbtree_t *mgrbtree_max_node(mgrbtree_t *root);

#endif
