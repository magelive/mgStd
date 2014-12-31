
#include <stdio.h>
#include <stdlib.h>
#include "mgrbtree.h"

#define NilNode NULL
#define MG_RBTREE_NODE_CMP(node1, node2)    (node1->cmp_func(node1, node2))
//LT nodo1 < node2       RT nodo1 > node2       EQ nodo1 = node2
#define MG_RBTREE_LT(node1, node2)          (MG_RBTREE_NODE_CMP(node1, node2) < 0)      
#define MG_RBTREE_RT(node1, node2)          (MG_RBTREE_NODE_CMP(node1, node2) > 0)
#define MG_RBTREE_EQ(node1, node2)          (MG_RBTREE_NODE_CMP(node1, node2)  == 0)

#define MG_RBTREE_IS_LCHILD(node)           ((node->parent != NilNode) && (node->parent->lchild == node))
#define MG_RBTREE_IS_RCHILD(node)           ((node->parent != NilNode) && (node->parent->rchild == node))


void mgrbtree_init(mgrbtree_t *node, mgrbtree_cmp_func *cmp_func)
{
    node->color     = mg_black;
    node->parent    = NilNode;
    node->lchild    = NilNode;
    node->rchild    = NilNode;
    node->cmp_func  = cmp_func;
}

void mgrbtree_preorder(mgrbtree_t *root, mg_rbtree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        opfunc(root, arg);
        mgrbtree_preorder(root->lchild, opfunc, arg);
        mgrbtree_preorder(root->rchild, opfunc, arg);
    }
}

//左旋
static mgrbtree_t* mgrbtree_left_rote(mgrbtree_t *root, mgrbtree_t *node)
{
    if (node == NilNode || node->rchild == NilNode)
        return root;

    mgrbtree_t *rchild = node->rchild;
    
    node->rchild = rchild->lchild;

    if (rchild->lchild != NilNode)
        rchild->lchild->parent = rchild;

    rchild->parent = node->parent;

    if(node->parent == NilNode)
    {
        root = rchild;
        root->parent = NilNode;
    }
    else if (MG_RBTREE_IS_LCHILD(node))
    {
        node->parent->lchild = rchild;
    }
    else
    {
        node->parent->rchild = rchild;
    }
    rchild->lchild = node;
    node->parent = rchild;
    return root;
}

//右旋
static mgrbtree_t *mgrbtree_right_rote(mgrbtree_t *root, mgrbtree_t *node)
{
    if (node == NilNode || node->lchild == NilNode)
        return root;
    mgrbtree_t *lchild = node->lchild; 
    
    node->lchild = lchild->rchild;
    if (lchild->rchild != NilNode)
        lchild->rchild->parent = node;
    
    lchild->parent = node->parent;

    if(node->parent == NilNode)
    {
        root = lchild;
    }
    else if(MG_RBTREE_IS_LCHILD(node))
    {
        node->parent->lchild = lchild;
    }
    else
    {
        node->parent->rchild = lchild;
    }
    lchild->rchild = node;
    node->parent = lchild;
    return root;
}


static mgrbtree_t *fixup_insert_mgrbtree_t(mgrbtree_t *root, mgrbtree_t *node)
{
    mgrbtree_t *uncle; 
    while(node->parent != NilNode && node->parent->color == mg_red)
    {
        if (MG_RBTREE_IS_LCHILD(node->parent))
        { 
            uncle = node->parent->parent->rchild;
            if(uncle && uncle->color == mg_red)
            { // case1
                node->parent->color = mg_black;
                uncle->color = mg_black;
                node->parent->parent->color = mg_red;
                node = node->parent->parent;
            }
            else
            {
                if (MG_RBTREE_IS_RCHILD(node))
                { //case 2
                    mgrbtree_t *np = node->parent;
                    root = mgrbtree_left_rote(root, np);   
                }
                // case 3
                node->parent->color = mg_black;
                if (node->parent->parent) node->parent->parent->color = mg_red;
                root = mgrbtree_right_rote(root, node->parent->parent);
            }

        } // if(node->parent->lchild == node)
        else
        { // p[z]为p[p[z]]的右结点
            if (node->parent->parent != NilNode)
            {
                uncle = node->parent->parent->lchild;
                if(uncle && uncle->color == mg_red)
                { // case4
                    node->parent->color = mg_black;
                    uncle->color = mg_black;
                    node->parent->parent->color = mg_red;
                    node = node->parent->parent;
                }
                else
                {
                    //if(node->parent->lchild == node)
                    if (MG_RBTREE_IS_LCHILD(node))
                    { // case5
                        mgrbtree_t *np = node->parent;
                        root = mgrbtree_right_rote(root, np);
                    }
                    // case6
                    node->parent->color = mg_black;
                    if (node->parent->parent) node->parent->parent->color = mg_red;
                    root = mgrbtree_left_rote(root, node->parent->parent);
                }
            }
        } // else
    }
    root->color = mg_black;   
    return root;
}


mgrbtree_t *mgrbtree_insert(mgrbtree_t *root, mgrbtree_t *node)
{
    mgrbtree_t *n_parent , *x_tmp ;
    n_parent = NilNode;
    x_tmp = root;
    // 找到插入node的父结点y~
    while(x_tmp != NilNode)
    {
        n_parent = x_tmp;
        (MG_RBTREE_RT(x_tmp, node) ? (x_tmp = x_tmp->lchild) : (x_tmp = x_tmp->rchild));
    }
    node->parent = n_parent;
    if(n_parent == NilNode)
    {
        root = node;
    }
    else if(MG_RBTREE_LT(node, n_parent))
    {
        n_parent->lchild = node;
    }
    else
    {
        n_parent->rchild = node;
    }

    node->lchild = node->rchild = NilNode;
    node->color = mg_red;
    root = fixup_insert_mgrbtree_t(root, node);

    return root;
}

/*
int delete_from_mgrbtree_t(mgrbtree_t &T, int key)
{
    mgrbtree_t z = NilNode;

    z = find_mgrbtree_t(T, key);
    if(z == NilNode)
    {
        return 0;
    }
    else
    {
        z = delete_from_mgrbtree_t(T, z);
        free(z);
        printf("delete the key:%d\n", key);
        pre_order_mgrbtree_t(T);

        return 1;
    }
}
*/

mgrbtree_t *mgrbtree_search(mgrbtree_t *root, mgrbtree_t *node)
{
    mgrbtree_t *search_node = root;
    
    while(search_node != NilNode)
    {
        if (MG_RBTREE_EQ(search_node, node))
        {
            break;
        }
        else if (MG_RBTREE_RT(search_node, node))
        {
            search_node = search_node->lchild;
        }
        else
        {
            search_node = search_node->rchild;
        }
    }
    return search_node;
}

static mgrbtree_t* mgrbtree_fixup_remove(mgrbtree_t *root, mgrbtree_t *node)
{
    mgrbtree_t *brother = NilNode;
    while(root !=node && node->color == mg_black)
    { // node为重黑结点，~
        if(MG_RBTREE_IS_LCHILD(node))//node == node->parent->lchild)
        { 
            brother = node->parent->rchild; // brother为node的兄弟~
            if (brother)
            {
                if(brother->color == mg_red)
                { // case 1
                    node->parent->color = mg_red;
                    brother->color = mg_black;
                    root = mgrbtree_left_rote(root, node->parent);
                }//if(brother->color == mg_red)
                else if(brother->rchild && brother->rchild->color == mg_black && 
                        brother->lchild && brother->lchild->color == mg_black)
                { // case 2
                    brother->color = mg_red;
                    node = node->parent;
                }//else if(brother->rchild && ...)
                else
                {
                    if(brother->rchild && brother->rchild->color == mg_black)
                    { // case 3 -> case4
                        brother->color = mg_red;
                        if (brother->lchild) brother->lchild->color = mg_black;
                        root = mgrbtree_right_rote(root, brother);
                        brother = node->parent->rchild;
                    }//if(brother->rchild && ...)
                    // case 4
                    if (brother->rchild) brother->rchild->color = mg_red;
                    brother->color = node->parent->color;
                    node->parent->color = mg_black;
                    root = mgrbtree_left_rote(root, node->parent);
                    node = root;
                }//else
            }//if (brother)
        } // if(node == node->parent->lchild)
        else
        { // node为node paret右孩子的情况~
            brother = node->parent->lchild;
            if (brother)
            {
                if(brother->color == mg_red)
                { // case 1'
                    node->parent->color = mg_red;
                    brother->color = mg_black;
                    root = mgrbtree_right_rote(root, node->parent);
                }
                else if(brother->lchild && brother->lchild->color == mg_black && 
                        brother->rchild && brother->rchild->color == mg_black)
                { // case 2'
                    brother->color = mg_red;
                    node = node->parent;
                }
                else
                {
                    if(brother->lchild && brother->lchild->color == mg_black)
                    { // case3'
                        brother->color = mg_red;
                        brother->rchild->color = mg_black;
                        root = mgrbtree_left_rote(root, brother);
                        brother = node->parent->lchild;
                    }
                    // case4'
                    if (brother->lchild) brother->lchild->color = mg_black;
                    brother->color = node->parent->color;
                    node->parent->color = mg_black;
                    root = mgrbtree_right_rote(root, node->parent);
                    node = root;
                }//else
            }//if (brother)
        }//  else{ // node为p[node]右孩子的情况~
    } // while(T!=node && node->color == mg_black)

    node->color = mg_black; // 或node为红黑结点~，或为根结点~
    return root;
}


static mgrbtree_t* __mgrbtree_remove_node_(mgrbtree_t *root, mgrbtree_t *node)
{
    if (node == root)
        return NilNode;
    // y为需要删除的指针; x为y的后继指针，~
    mgrbtree_t *x = NilNode;
    mgrbtree_t *y = NilNode;

    if(node->lchild == NilNode || node->rchild == NilNode)
    { // 如果node为单子树
        y = node;
    }
    else
    { // 找到node的后继，~即右树子的最左结点，~
        y = node->rchild;
        while(y->lchild != NilNode) 
        {
            y = y->lchild;
        }
    }
    // 找到y的后继
    if(y->lchild != NilNode)
    {
        x = y->lchild;
    }
    else
    {
        x = y->rchild;
    }
   
    //删除y
    if (x) x->parent = y->parent;
    if(y->parent == NilNode)
    {// y即为根结点
        root = x;
    }
    else if(MG_RBTREE_IS_LCHILD(y))//(y->parent->lchild == y)
    {
        y->parent->lchild = x;
    }
    else
    {
        y->parent->rchild = x;
    }

    //将y的颜色记录下来
    mgrbtree_color_t del_color = y->color;
    //将y替换掉node,再将node解除
    if(y!=node)
    {
        //node->key = y->key;    
        y->parent = node->parent;
        y->lchild = node->lchild;
        y->rchild = node->rchild;
        y->color  = node->color;

        (MG_RBTREE_IS_LCHILD(node) ? (node->parent->lchild = y) : (node->parent->rchild = y));
        if (node->lchild) node->lchild->parent = y;
        if (node->rchild) node->rchild->parent = y;
        
        mgrbtree_init(node, node->cmp_func);
    }
    
    if(del_color == mg_black && x!= NilNode) root = mgrbtree_fixup_remove(root, x);

    return root;
}

mgrbtree_t *mgrbtree_remove(mgrbtree_t *root, mgrbtree_t *node)
{
    mgrbtree_t *search_node = mgrbtree_search(root, node);
    root = __mgrbtree_remove_node_(root, search_node);
    node->rchild = search_node;
    return root;
}

