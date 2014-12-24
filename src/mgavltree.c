#include <stdio.h>
#include <stdlib.h>
#include "mgavltree.h"

static unsigned long long mg_avltree_height(MG_AVLTREE_NODE *node)
{
    return MG_AVLTREE_HEIGHT(node);
}

/*
 * 前序遍历
 */
void mg_preorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        opfunc(root, arg);
        mg_preorder_avltree(root->lchild, opfunc, arg);
        mg_preorder_avltree(root->rchild, opfunc, arg);
    }
}


/*
 * 中序遍历
 */
void mg_inorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        mg_inorder_avltree(root->lchild, opfunc, arg);
        opfunc(root, arg);
        mg_inorder_avltree(root->rchild, opfunc, arg);
    }
}

/*
 * 后序遍历
 */
void mg_postorder_avltree(MG_AVLTREE_NODE *root, mg_avltree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        mg_postorder_avltree(root->lchild, opfunc, arg);
        mg_postorder_avltree(root->rchild, opfunc, arg);
        opfunc(root, arg);
    }
}

/*
 * (递归实现)查找
 */
MG_AVLTREE_NODE* mg_avltree_search_recursive(MG_AVLTREE_NODE *root, MG_AVLTREE_NODE *find_node)
{
    if (root == NULL || find_node == NULL)
        return NULL;
    int result = root->cmp_func(root, find_node);
    if (return == 0)
        return root;
    else if (result > 0) //root->key > find_node->key
        return mg_avltree_search(root->lchild, node);
    else
        return mg_avltree_search(root->rchild, node);
}

/*
 * (非递归实现)查找
 */
MG_AVLTREE_NODE* mg_avltree_search(MG_AVLTREE_NODE* root, MG_AVLTREE_NODE *find_node)
{
    if (root == NULL || find_node == NULL)
        return NULL;
    MG_AVLTREE_NODE * x = root;
    int result = 0;
    while (x != NULL)
    {
        result = x->cmp_func(x, find_node);
        if (result == 0)
            break;
        else if (result > 0)
            x = x->lchild;
        else
            x = x->rchild;
    }
    return x;
}

/* 
 * 查找最小结点
 */
MG_AVLTREE_NODE* mg_avltree_min_node(MG_AVLTREE_NODE* root)
{
    if (root == NULL)
        return NULL;

    while(root->lchild != NULL)
        root = root->lchild;
    return root;
}

/* 
 * 查找最大结点
 */
MG_AVLTREE_NODE* mg_avltree_max_node(MG_AVLTREE_NODE* root)
{
    if (root == NULL)
        return NULL;

    while(root->rchild != NULL)
        root = root->rchild;
    return root;
}

/*
 * LL：左左情况:
 *      插入或删除一个节点后，节点的左子树的左子树还有非空子节点，
 *      导致"根的左子树的高度"比"根的右子树的高度"大2，导致AVL树失去了平衡。
 *平衡算法
 *  进行右旋转
 * 返回值：旋转后的根节点
 */
static MG_AVLTREE_NODE* left_left_rotation(MG_AVLTREE_NODE* node)
{
    MG_AVLTREE_NODE* nl;
    MG_AVLTREE_NODE* parent = node->parent;

    nl = node->lchild;
    node->lchild = nl->rchild;
    nl->rchild = node;

    node->lchild->parent = node;
    nl->parent = node->parent;
    node->parent = nl;

    (parent->lchild == node) ? (parent->lchild = nl):(parent->rchild = nl);

    node->avltree_height = MG_MAX( MG_AVLTREE_HEIGHT(node->lchild), MG_AVLTREE_HEIGHT(node->rchild)) + 1;
    nl->avltree_height = MG_MAX( MG_AVLTREE_HEIGHT(nl->lchild), MG_AVLTREE_NODE(node)) + 1;

    return nl;
}

/*
 * RR：右右情况：
 *      插入或删除一个节点后，根节点的右子树的右子树还有非空子节点，
 *      导致"根的右子树的高度"比"根的左子树的高度"大2，导致AVL树失去了平衡。
 *平衡算法：
 *      进行左旋转
 * 返回值：旋转后的根节点
 */
static MG_AVLTREE_NODE* right_right_rotation(MG_AVLTREE_NODE* node)
{
    MG_AVLTREE_NODE* nr;
    MG_AVLTREE_NODE* parent = node->parent;

    nr = node->rchild;
    node->rchild = nr->lchild;
    nr->lchild = node;

    node->rchild->parent = node;
    nr->parent = node->parent;
    node->parent = nr;

    (parent->lchild == node) ? (parent->lchild = nr):(parent->rchild = nr );

    node->height = MG_MAX( MG_AVLTREE_HEIGHT(node->lchild), MG_AVLTREE_HEIGHT(node->rchild)) + 1;
    nr->height = MG_MAX( MG_AVLTREE_HEIGHT(nr->rchild), MG_AVLTREE_HEIGHT(node)) + 1;
    return nr;
}

/*
 * LR：左右情况：
 *      插入或删除一个节点后，根节点的左子树的右子树还有非空子节点，
 *      导致"根的左子树的高度"比"根的右子树的高度"大2，导致AVL树失去了平衡。
 *平衡算法：
 *      先进行其左结点进行左旋，再对其进行右旋
 * 返回值：旋转后的根节点
 */
static MG_AVLTREE_NODE* left_right_rotation(MG_AVLTREE_NODE* node)
{
    node->lchild = right_right_rotation(node->lchild);
    return left_left_rotation(node);
}

/*
 * RL：右左情况：
 *      插入或删除一个节点后，根节点的右子树的左子树还有非空子节点，
 *      导致"根的右子树的高度"比"根的左子树的高度"大2，导致AVL树失去了平衡。
 *平衡算法：
 *      先对其右结点进行右旋，再对其进行左旋
 * 返回值：旋转后的根节点
 */
static MG_AVLTREE_NODE* right_left_rotation(MG_AVLTREE_NODE* node)
{
    node->rchild = left_left_rotation(node->rchild);

    return right_right_rotation(node);
}
/* 
 * 插入
 *
 * 参数说明：
 *     root     根结点
 *     node     待插入的结点
 * 返回值：
 *     根节点
 */
MG_AVLTREE_NODE* mg_avltree_insert(MG_AVLTREE_NODE* root, MG_AVLTREE_NODE *node)
{
    if (node == NULL)
        return root;
    if (root == NULL) 
    {
        root = node;
        root->avltree_height = 0;
        root->parent = NULL;
        root->lchild = NULL;
        root->rchild = NULL;
    }
    else
    {
        int result = root->cmp_func(root, node);
        if (result > 0)//root>node
        {
            root->lchild = mg_avltree_insert(root->lchild, node);
            if (root->lchild == node)
            {
                node->parent = root;
            }
            //插入节点后，若失去平衡，则进行调节
            if (MG_AVLTREE_HEIGHT(root->lchild) - MG_AVLTREE_HEIGHT(root->rchild) == 2)
            {
                int rs = root->lchild->cmp_func(root->lchild, node);
                if (rs > 0)//root->lchild > node, 插入的节点小于其左结点，节点插入在其左结点的左边，造成LL情况
                    root = left_left_rotation(root);
                else//造成LR情况
                    root = left_right_rotation(root);
            }
        }
        else if (result < 0) //root<node 
        {
            root->rchild = mg_avltree_insert(root->rchild, node);
            if (root->rchild == node)
            {
                node->parent = root;
            }
            //插入节点后，若失去平衡，则进行调节
            if (MG_AVLTREE_HEIGHT(root->rchild) - MG_AVLTREE_HEIGHT(root->lchild) == 2)
            {
                int rd = root->rchild->cmp_func(root->rchild, node);
                if (rd < 0 ) // root->rchild < node, 
                    root = right_right_rotation(root);
                else
                    root = right_left_rotation(root);
            }
        }
        else //node == root
        {
            //相同结点不做任何处理，直接返回
            return root;
        }
    }
    root->height = MG_MAX( MG_AVLTREE_HEIGHT(root->lchild), MG_AVLTREE_HEIGHT(root->rchild)) + 1;
    return root;
}

