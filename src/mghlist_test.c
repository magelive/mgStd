#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "mgMurmurhash.h"
#include "mglist.h"
#include "mgtime.h"

struct mghlist_test_node
{
    int value;
    struct mghlist_node test_node;
};

struct mghlist_test_line
{
   int key;
   struct mghlist_head test_mghlist;
   //struct mghlist_test_line *next;
   struct mglist_head test_mglist;
};

#define MG_HLIST_TEST_LEN 10

int simple_hash_func(int key)
{
    int value = key%MG_HLIST_TEST_LEN;
    return value;
}

int main(int argc, char *argv[])
{
    struct mghlist_test_line table_line[MG_HLIST_TEST_LEN];
    struct mghlist_test_node line_node[MG_HLIST_TEST_LEN * 10];
    int i =0;
    MG_LIST_HEAD(table_list_head);
    struct mghlist_test_line *table_pos = NULL; 
    for(i = 0; i < MG_HLIST_TEST_LEN; i++)
    {
        table_line[i].key = i;
        INIT_MG_LIST_HEAD(&table_line[i].test_mglist);
        table_line[i].test_mghlist.first = NULL;
        mglist_add_tail(&table_line[i].test_mglist, &table_list_head);
    }
    
    int tmp_key = -1;
    int j=0, k = 0;
    for (i = 0; i < MG_HLIST_TEST_LEN*10; i++)
    {
        INIT_MG_HLIST_NODE(&line_node[i].test_node);
        line_node[i].value = mg_get_rand_num(1, MG_HLIST_TEST_LEN*10);
        tmp_key = simple_hash_func(line_node[i].value);
        table_pos = NULL;
        mglist_for_each_entry(table_pos, &table_list_head, test_mglist)
        {
            if (table_pos->key == tmp_key)
            {
                mghlist_add_head(&line_node[i].test_node, &table_pos->test_mghlist);
            }
        }
        usleep(100);
    }
    struct mghlist_test_node *node_pos = NULL;

    //out
    printf("Out all members: \n");
    i = 0;j=0;
    mglist_for_each_entry(table_pos, &table_list_head, test_mglist)
    {
        printf("line %d key %d :", ++i, table_pos->key);
        mghlist_for_each_entry(node_pos, &table_pos->test_mghlist, test_node)
        {
            j++;
            printf(" %d ", node_pos->value);
        }
        printf("\n\n");
    }
    printf("num count = %d\n", j); 
    //find
    int find_number = mg_get_rand_num(0, MG_HLIST_TEST_LEN*10-1);
    printf("find number = %d\n", line_node[find_number].value);
    int find_key = simple_hash_func(line_node[find_number].value);
    i =0;k=0;
    mglist_for_each_entry(table_pos, &table_list_head, test_mglist)
    {
        i++;
        if (find_key == table_pos->key)
        {
            mghlist_for_each_entry(node_pos, &table_pos->test_mghlist, test_node)
            {
                k++;
                if(node_pos->value == line_node[find_number].value)
                {
                    break;
                }
            }
            if (k != 0)
                break;
        }
    }
    printf("line_node[%d].value %d ,  pos : %d, %d\n", find_number, line_node[find_number].value, i, k);

    printf("############################################################\n");
    return 0;
}
