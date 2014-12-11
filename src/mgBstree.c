#include <stdio.h>
#include <stdlib.h>

#include "mgBstree.h"

void MG_BSTREE_INIT_NODE(MG_BS_TREE *node, mg_BStree_cmp_func *cmp)
{
    node->parent = NULL;
    node->rchild = NULL;
    node->lchild = NULL;
    node->cmp_func = cmp;
}

static void MG_BSTREE_SET_PARENT_CHILD(MG_BS_TREE *node, MG_BS_TREE *child)
{
    ((node->parent == NULL) ? node=child :
           ((node == node->parent->lchild)?(node->parent->lchild=child):(node->parent->rchild=child)));
    return;
}

void mg_Bstree_insert(MG_BS_TREE *root, MG_BS_TREE *node)
{
   if (node == NULL)
       return;
   if (root == NULL)
   {
       root = node;
       MG_BSTREE_INIT_NODE(root, node->cmp_func);
       return;
   }
   int cmp_result = root->cmp_func(root, node);
   if (cmp_result < 0)//root<node
   {
        if (root->rchild == NULL)
        {
            MG_BSTREE_INSERT_RIGHT(root, node);
        }
        else
        {
            mg_Bstree_insert(root->rchild, node);
        }
   }
   else if (cmp_result > 0)
   {
        if (root->lchild == NULL)
        {
            MG_BSTREE_INSERT_LEFT(root, node);
        }
        else
        {
            mg_Bstree_insert(root->lchild, node);
        }
   }
   return;
}

void mg_Bstree_del(MG_BS_TREE *root, MG_BS_TREE *node)
{
    MG_BS_TREE *tmp;
    MG_BS_TREE *delnode = node;
    if (root == NULL || node == NULL)
        return;
    if (node->lchild == NULL || node->rchild == NULL)
    {
        if (node->lchild != NULL)
        {
            MG_BSTREE_SET_PARENT_CHILD(node, node->lchild);
        }
        else if (node->rchild != NULL)
        {
            MG_BSTREE_SET_PARENT_CHILD(node, node->rchild);
        }
        else
        {
            MG_BSTREE_SET_PARENT_CHILD(node, NULL);
        }
    }
    else
    {
        tmp = node->rchild;
        MG_BSTREE_SET_PARENT_CHILD(node, node->rchild);
        tmp = tmp->lchild;
        while(tmp != NULL)
        {
            tmp = tmp->lchild;
        }
        tmp->parent->lchild = delnode->lchild;
    }
}

MG_BS_TREE *mg_Bstree_find(MG_BS_TREE *root, MG_BS_TREE *node)
{
    int cmp_result = 0;
    MG_BS_TREE *p = root;
    while(p)
    {
        cmp_result = p->cmp_func(p, node);
        if (cmp_result == 0)
            break;
        else if (cmp_result > 0)    //p>node
            p = p->lchild;
        else
            p = p->rchild;            
    }
    return p;
}

void mg_Bstree_find_recursive(MG_BS_TREE *root, MG_BS_TREE *node, MG_BS_TREE **result)
{
    int cmp_result;
    if (root)
    {
        cmp_result = root->cmp_func(root, node);
        if (cmp_result == 0)
        {
            *result = root;
            return;
        }
        else if (cmp_result > 0)
            mg_Bstree_find_recursive(root->lchild, node, result);
        else
            mg_Bstree_find_recursive(root->rchild, node, result);
    }
    return;
}

void mg_Bstree_visit_LMR(MG_BS_TREE *root, mg_BStree_op_func op_func, void *arg)
{
    if (root)
    {
        mg_Bstree_visit(root->lchild, op_func, arg);
        op_func(root, arg);
        mg_Bstree_visit(root->rchild, op_func, arg);
    }
}
    
void mg_Bstree_visit_MLR(MG_BS_TREE *root, mg_BStree_op_func op_func, void *arg)
{
    if (root)
    {
        op_func(root, arg);
        mg_Bstree_visit(root->lchild, op_func, arg);
        mg_Bstree_visit(root->rchild, op_func, arg);
    }
}

void mg_Bstree_visit_LRM(MG_BS_TREE *root, mg_BStree_op_func op_func, void *arg)
{
    if (root)
    {
        mg_Bstree_visit(root->lchild, op_func, arg);
        mg_Bstree_visit(root->rchild, op_func, arg);
        op_func(root, arg);
    }
}
