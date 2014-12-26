#include <stdio.h>
#include <stdlib.h>

#include "mgsplaytree.h"

void mg_splaytree_node_init(mg_splaytree_t *node, mg_splaytree_node_cmp_func *cmp_func)
{
    node->parent = NULL;
    node->lchild = NULL;
    node->rchild = NULL;
    node->cmp_func = cmp_func;
}

//中序遍历 MLR
void mg_preorder_splaytree(mg_splaytree_t *root, mg_splaytree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        opfunc(root, arg);
        mg_preorder_splaytree(root->lchild, opfunc, arg);
        mg_preorder_splaytree(root->rchild, opfunc, arg);
    }
}


//前序遍历  LMR
void mg_inorder_splaytree(mg_splaytree_t *root, mg_splaytree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        mg_inorder_splaytree(root->lchild, opfunc, arg);
        opfunc(root, arg);
        mg_inorder_splaytree(root->rchild, opfunc, arg);
    }
}

//后序遍历 LRM
void mg_postorder_splaytree(mg_splaytree_t *root, mg_splaytree_op opfunc, void *arg)
{
    if(root != NULL)
    {
        mg_postorder_splaytree(root->lchild, opfunc, arg);
        mg_postorder_splaytree(root->rchild, opfunc, arg);
        opfunc(root, arg);
    }
}

//查找最小结点
mg_splaytree_t* mg_splaytree_min_node(mg_splaytree_t* root)
{
    if (root == NULL)
        return NULL;

    while(root->lchild != NULL)
        root = root->lchild;
    return root;
}

//查找最大结点
mg_splaytree_t* mg_splaytree_max_node(mg_splaytree_t* root)
{
    if (root == NULL)
        return NULL;

    while(root->rchild != NULL)
        root = root->rchild;
    return root;
}

//在树中搜寻某值
mg_splaytree_t *mg_splaytree_search(mg_splaytree_t *root, mg_splaytree_t *node)
{
    if (!root)
        return NULL;
    int cmp_result = root->cmp_func(root, node);
    if (cmp_result > 0)//root>node
    {
        mg_splaytree_search(root->lchild, node);
    }
    else if (cmp_result < 0)
    {
        mg_splaytree_search(root->rchild, node);
    }
    else
    {
        return root;
    }
}

//右单旋
static mg_splaytree_t* right_single_rotation(mg_splaytree_t* node)
{
    mg_splaytree_t* nl;
    mg_splaytree_t* parent = node->parent;

    if (node->lchild == NULL)
        return node;

    nl = node->lchild;
    node->lchild = nl->rchild;
    nl->rchild = node;

    if (node->lchild != NULL)
        node->lchild->parent = node;
    nl->parent = node->parent;
    node->parent = nl;

    if (parent != NULL)
        (parent->lchild == node) ? (parent->lchild = nl):(parent->rchild = nl);

    return nl;
}

//左单旋
static mg_splaytree_t* left_single_rotation(mg_splaytree_t* node)
{
    mg_splaytree_t* nr;
    mg_splaytree_t* parent = node->parent;

    if (node->rchild == NULL)
        return node;

    nr = node->rchild;
    node->rchild = nr->lchild;
    nr->lchild = node;

    if (node->rchild != NULL)
        node->rchild->parent = node;

    nr->parent = node->parent;
    node->parent = nr;

    if (parent != NULL)
        (parent->lchild == node) ? (parent->lchild = nr):(parent->rchild = nr );

    return nr;
}

 
//右双旋
static mg_splaytree_t* right_double_rotation(mg_splaytree_t* node)
{
    node->lchild = left_single_rotation(node->lchild);
    return right_single_rotation(node);
}

//左双旋
static mg_splaytree_t* left_double_rotation(mg_splaytree_t* node)
{
    node->rchild = right_single_rotation(node->rchild);

    return left_single_rotation(node);
}

//自下向上伸展,这个可以保证伸展结束后，root为node的一个子树
static mg_splaytree_t *mg_splaytree_splay_node_to_root(mg_splaytree_t *root, mg_splaytree_t *node)
{
    mg_splaytree_t *seach_node = mg_splaytree_search(root, node);
    if (seach_node == NULL)
    {
        return root;
    }
    while(1)
    {
        if (seach_node->parent == NULL)
        {
            break;
        }
        
        if (seach_node->parent->lchild == seach_node)
        {
            seach_node = right_single_rotation(seach_node->parent);
        }
        else
        {
            seach_node = left_single_rotation(seach_node->parent);
        }
    }
    return seach_node;
}
//自顶向下伸展，只保证伸展结点后，node在为根结点或与node值相近的值为根结点
static mg_splaytree_t *mg_splaytree_splay_root_to_node(mg_splaytree_t *root, mg_splaytree_t *node)
{
    if (!root || !node)
        return root;
    int cmp_result = 0, cr = 0;
    while(1)
    {
        cmp_result = root->cmp_func(root, node);
        if (cmp_result > 0) // root > node, 往root的左子树去找
        {
            if (root->lchild == NULL)
            {
                break;
            }
            cmp_result = root->lchild->cmp_func(root->lchild, node);
            if (cmp_result >= 0)//root->lchild >= node
            {
                root = right_single_rotation(root);
            }
            else
            {
                root->lchild = left_single_rotation(root->lchild);
            }
        }
        else if (cmp_result < 0)
        {
            if (root->rchild == NULL)
                break;
            cmp_result = root->rchild->cmp_func(root->rchild, node);
            if (cmp_result <= 0)
            {
                root = left_single_rotation(root);
            }
            else
            {
                root->rchild = right_single_rotation(root->rchild);
            }
        }
        else
        {
            break;
        }
    }
    return root;
}

mg_splaytree_t* mg_splaytree_find(mg_splaytree_t *root, mg_splaytree_t *node)
{
    root = mg_splaytree_splay_node_to_root(root, node);
    return root;
}

mg_splaytree_t* mg_splaytree_insert(mg_splaytree_t* root, mg_splaytree_t *node)
{
    if (!node)
        return root;
    int cmp_result = 0;
    root = mg_splaytree_splay_node_to_root(root, node);
    if (!root)
    {
        root = node;
        return root;
    }
    cmp_result = root->cmp_func(root, node);
    if (cmp_result > 0)//root > node
    {
        node->rchild = root;
        root->parent = node;
        node->parent = NULL;
        node->lchild = NULL;
        root = node;
    }
    else if (cmp_result < 0)//root < node
    {
        node->lchild = root;
        root->parent = node;
        node->parent = NULL;
        node->rchild = NULL;
        root = node;
    }
    else // root = node
    {
        //exist node
    }
    return root;
    
}

mg_splaytree_t *mg_splaytree_remove(mg_splaytree_t *root, mg_splaytree_t *node, mg_splaytree_node_free *nfree)
{
    if (!root || !node)
    {
        return root;
    }
    int cmp_result = 0;
    root = mg_splaytree_splay_node_to_root(root, node);
    cmp_result = root->cmp_func(root, node);
    if (cmp_result != 0)
    {
        node->rchild = NULL;
    }
    else
    {
        node->rchild = root;
        if (root->lchild && root->rchild)
        {
            mg_splaytree_t *min = mg_splaytree_min_node(root->rchild);
            min->lchild = root->lchild;
            root->lchild->parent = min;
            root = root->rchild;
        }
        else
        {
            ((root->lchild == NULL) ? (root = root->rchild) : (root = root->lchild));
        }
    }
    (*nfree)(node->rchild);
    return root;
}

void mg_splaytree_destory(mg_splaytree_t *root, mg_splaytree_node_free nfree)
{
    if (root)
    {
        mg_splaytree_destory(root->lchild, nfree);
        mg_splaytree_destory(root->rchild, nfree);
        nfree(root);
    }
}


