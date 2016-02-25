#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "mgMurmurhash.h"
#include "mglist.h"
#include "mgtime.h"

#define MURMURHASH_SEED 16


typedef struct mghashtable_line_node
{
    void    *key;
    void    *data;
    int     key_len;
    int     data_len;
    int     murm_key;
    MG_LIST list_node;
}mghashtable_line_node;

typedef struct mghashtable_line
{
    void *key;
    int key_len;
    MG_LIST list_head;
}mghashtable_line;

typedef struct mghashtable mghashtable;

typedef int (mghashtable_key_cmp)(mghashtable *table, void *key1, int key1_len, void *key2, int key2_len);

struct mghashtable
{
    int                 line_num;
    mghashtable_line    *lines;
    mghashtable_key_cmp *key_cmp;
};


mghashtable_line_node* node_init_num(int num)
{
    mghashtable_line_node *node = (mghashtable_line_node *)calloc(num, sizeof(mghashtable_line_node));
    if (node)
    {
        bzero(node, num*sizeof(mghashtable_line_node));
        int i = 0;
        while(i < num)
        {
            mglist_init(&node[i++].list_node);
        }
    }
    return node;
}

mghashtable_line_node* node_init(void *key, int key_len, void *data, int data_len)
{
    mghashtable_line_node *node = NULL;
    if (key && key_len > 0)
    {
        node = node_init_num(1);
        if (node)
        {
            node->key = (void *)malloc(key_len);
            node->key_len = key_len;
            memcpy(node->key, key, key_len);
            if (data && data_len > 0)
            {
                node->data = (void *)malloc(data_len);
                node->data_len = data_len;
                memcpy(node->data, data, data_len);
            }
            node->murm_key = mgMurmurhash32(key, key_len, MURMURHASH_SEED);
            mglist_init(&node->list_node);
        }
    }
    return node;
}

int node_replace_data(mghashtable_line_node *node, void *data, int data_len)
{
    if (data && data_len > 0)
    {
        node->data = (void *)realloc(node->data, data_len);
        if (node->data)
        {
            node->data_len = data_len;
            bzero(node->data, node->data_len);
            memcpy(node->data, data, data_len);
            return 1;
        }
    }
    return 0;
}

void node_free(mghashtable_line_node *node)
{
    if(node)
    {
        if (node->key)
        {
            free(node->key);
            node->key = NULL;
        }
        if (node->data)
        {
            free(node->data);
            node->data = NULL;
        }
        free(node);
    }
}

#define line_init() line_init_num(1)
mghashtable_line *line_init_num(int num)
{
    mghashtable_line *line = (mghashtable_line *)calloc(num, sizeof(mghashtable_line));
    if (line)
    {
        bzero(line, num*sizeof(mghashtable_line));
        int i = 0;
        while (i < num)
        {
            mglist_init(&(line[i++].list_head));
        }
        /*
        mghashtable_line *curline = line;
        while(curline < line+(num*sizeof(mghashtable_line)))
        {
            curline += sizeof(mghashtable_line);
        }
        */
    }
    return line;
}

void mghashtable_set_line_key(mghashtable *table, void *keys, int *key_len)
{
    if ( table != NULL )
    {
        int i = 0;
        mghashtable_line *curline = table->lines;
        void *curkey = keys;
        while(curline && i < table->line_num )
        {
            if (key_len && keys)
            {
                curline[i].key_len = key_len[i];
                curline[i].key = (void *)malloc(key_len[i]);
                memcpy(curline[i].key, curkey, key_len[i]);
                curkey += key_len[i];
            }
            else
            {
                curline[i].key_len = sizeof(int);
                curline[i].key = (void *)malloc(sizeof(int));
                *(int *)curline[i].key = i;
            }
            i ++;
        }
    }
}

void line_free(mghashtable_line *line)
{
    if (line)
    {
        if (line->key)
            free(line->key);
        MG_LIST *pos = NULL;
        MG_LIST *n = NULL;
        mghashtable_line_node *nodepos = NULL;
        mglist_for_each_safe(pos, n, &line->list_head)
        {
            nodepos = mglist_entry(pos, mghashtable_line_node, list_node);
            mglist_del(pos);
            node_free(nodepos);
        }
    }
}

int default_key_cmp(mghashtable *table, void *key1, int key1_len, void *key2, int key2_len)
{
    if (table)
    {
        int num1 = *(int *)key1;
        int num2 = (*(int *)key2)%(table->line_num);
        if (num1 == num2)
            return 1;
    }
    return 0;
}

mghashtable *mghashtable_init(int num, mghashtable_key_cmp *cmpfun, void *lines_key, int *keys_len)
{
    mghashtable *table = (mghashtable *)malloc(sizeof(mghashtable));
    if (table)
    {
        table->line_num = num;
        table->lines = line_init_num(num);
        mghashtable_set_line_key(table, lines_key, keys_len);
        if (cmpfun)
            table->key_cmp = cmpfun;
        else
            table->key_cmp = default_key_cmp;
    }
    return table;
}

void mghashtable_finish(mghashtable **table)
{
    if (*table)
    {
        if ((*table)->lines)
        {
            int i = 0; 
            for (i = 0; i < (*table)->line_num; i++)
            {
               line_free(&(*table)->lines[i]); 
            }
            free((*table)->lines);
        }
        free(*table);
        *table = NULL;
    }
}

mghashtable_line *mghashtable_get_line(mghashtable *table, void *key, int key_len)
{
    mghashtable_line *get_line = NULL;
    if (table && key && key_len)
    {
        int i = 0;
        for ( i = 0; i < table->line_num; i ++)
        {
            mghashtable_line *tmp_line = &(table->lines[i]);
            if (table->key_cmp(table, tmp_line->key, tmp_line->key_len, key, key_len))
            {
                get_line = tmp_line;
                break;
            }
        }
    }
    return get_line;
}

int mghashtable_add_node(mghashtable *table, void *key, int key_len, void *data, int data_len)
{
    if ( table && key && key_len > 0 )
    {
        mghashtable_line *line = mghashtable_get_line(table, key, key_len);
        if (line)
        {
            mghashtable_line_node *node = node_init(key, key_len, data, data_len);
            if (node)
            {
                mglist_add_tail(&node->list_node, &line->list_head);
                return 1;
            }
        }
    }
    return 0;
}

mghashtable_line_node *mghashtable_get_node(mghashtable *table, void *key, int key_len)
{
    mghashtable_line_node *node = NULL;
    if (table && key && key_len > 0)
    {
        mghashtable_line *line = mghashtable_get_line(table, key, key_len);
        if (line && !mglist_empty(&line->list_head))
        {
            mghashtable_line_node *nodepos = NULL;
            mglist_for_each_entry(nodepos, &line->list_head, list_node)
            {
                if (nodepos->key_len == key_len &&
                        memcmp(nodepos->key, key, key_len) == 0)
                {
                    node = nodepos;
                    break;
                }
            }
        }
    }
    return node;
}

mghashtable_line_node* mghashtable_remove_node(mghashtable *table, void *key, int key_len)
{
    mghashtable_line_node *node = NULL;
    if (table && key && key_len > 0)
    {
        node = mghashtable_get_node(table, key, key_len);
        if (node)
        {
            mglist_del(&node->list_node);
        }
    }
    return node;
}

int mghashtable_put_data(mghashtable *table, void *key, int key_len, void *data, int data_len)
{
    int result = 0;
    if ( table && key && key_len > 0)
    {
        mghashtable_line_node *node = mghashtable_get_node(table, key, key_len);
        if (node)
        {
            result = node_replace_data(node, data, data_len);
        }
        else
        {
            result = mghashtable_add_node(table, key, key_len, data, data_len);
        }
    }
    return result;
}

void *mghashtable_get_data(mghashtable *table, void *key, int key_len, int *data_len)
{
    void *data = NULL;
    if (table && key && key_len > 0)
    {
        mghashtable_line_node *node = mghashtable_get_node(table, key, key_len);
        if (node)
        {
            data = node->data;
            *data_len = node->data_len;
        }
    }
    return data;
}

#define _SK(str)    (char *)str, (str==NULL?0:strlen(str))
#define _IK(num)    &(int *)num, sizeof(int)
#define _FK(fnum)   &(double *)fnum, sizeof(double)
#define _K  _SK

int mghashtable_put_int(mghashtable *table, void *key, int key_len, int data)
{
    return mghashtable_put_data(table, key, key_len, &data, sizeof(int));
}

int mghashtable_get_int(mghashtable *table, void *key, int key_len)
{
    int len = 0;
    void *data = mghashtable_get_data(table, key, key_len, &len);
    if (!data || len != sizeof(int))
        return 0;
    return (*(int *)data);
}

int mghashtable_put_str(mghashtable *table, void *key, int key_len, char *data)
{
    int data_len = data==NULL?0:strlen(data);
    return mghashtable_put_data(table, key, key_len, data, data_len);
}

char *mghashtable_get_str(mghashtable *table, void *key, int key_len)
{
    int len = 0;
    char *str = NULL;
    void *data = mghashtable_get_data(table, key, key_len, &len);
    if (data && len > 0)
    {
        str = (char *)malloc(len+1);
        bzero(str, len+1);
        memcpy(str, data, len);
    }
    return str;
}
#define _MG_DEBUG
#ifdef _MG_DEBUG
void print_hex(void *data, int data_len)
{
    if (!data || data_len <= 0) return;
    int i = 0;
    while( i < data_len )
    {
        printf("%02x", ((unsigned char *)data)[i++]);
    }
}
void print_node(mghashtable_line_node *node)
{
    if (node)
    {
        if (node->key)
            print_hex(node->key, node->key_len);
        else
            printf("KEY_NONE");
        printf(":");
        if (node->data)
            print_hex(node->data, node->data_len);
        else
            printf("DATA_NONE");
    }
    else
    {
        printf("KEY:DATA");
    }

}

void print_line(mghashtable_line *line)
{
    if (line)
    {
        printf("[");
        if (line->key)
            print_hex(line->key, line->key_len);
        else
            printf("line not have key");
        printf("] ");
        if (!mglist_empty(&line->list_head))
        {
            mghashtable_line_node *nodepos = NULL;
            mglist_for_each_entry(nodepos, &line->list_head, list_node)
            {
                print_node(nodepos);
                printf(" ");
            }
        }
    }
    else
    {
        printf("Line is None.");
    }
    printf("\n");
}

void print_table(mghashtable *table)
{
    printf("------------------------------\n");
    if (table)
    {
        printf("hashtable data...\n");
        printf("this table has %d lines.\n", table->line_num);
        int i = 0;
        for (i = 0; i < table->line_num; i ++ )
        {
            printf("line %d :", i);
            print_line(&table->lines[i]);
        }
    }
    else
    {
        printf("table is null.\n");
    }
    printf("==============================\n");
}

int test1_cmp(mghashtable *table, void *key1, int key_len, void *key2, int key2_len)
{
    char a1 = *(char *)key1;
    char a2 = *(char *)key2;
    if (a1 == a2)
        return 1;
    return 0;
}
void test_hashtable1()
{
    char *keys = "abcd";
    int keys_len[4];
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        keys_len[i] = 1;
    }
    mghashtable *table = mghashtable_init(4, test1_cmp, (void *)keys, keys_len);
    print_table(table);

    mghashtable_put_int(table, _K("a1"), 1);
    mghashtable_put_int(table, _K("b1"), 11);
    mghashtable_put_int(table, _K("c1"), 21);
    mghashtable_put_int(table, _K("a2"), 2);
    mghashtable_put_int(table, _K("b2"), 12);
    mghashtable_put_int(table, _K("c2"), 22);
    mghashtable_put_int(table, _K("c21"), 221);
    print_table(table);

    int a1 = mghashtable_get_int(table, _K("c21"));
    printf("a1 = %d\n", a1);
    mghashtable_finish(&table);
    print_table(table);
}

void test_hashtable2()
{
    mghashtable *table = mghashtable_init(16, NULL, NULL, NULL);
    print_table(table);
    mghashtable_put_int(table, _K("a1"), 1);
    mghashtable_put_int(table, _K("b1"), 11);
    mghashtable_put_int(table, _K("c1"), 21);
    mghashtable_put_int(table, _K("a2"), 2);
    mghashtable_put_int(table, _K("b2"), 12);
    mghashtable_put_int(table, _K("c2"), 22);
    mghashtable_put_int(table, _K("c21"), 221);
    mghashtable_put_int(table, _K("h21"), 421);
    mghashtable_put_int(table, _K("21"), 0x21);
    print_table(table);


    mghashtable_put_str(table, _K("DFSDF"), "dafsdfadf");
    print_table(table);
    char *str = mghashtable_get_str(table, _K("DFSDF"));
    printf("str = %s\n", str);
    free(str);

    mghashtable_put_int(table, _K("a1"), 15);
    print_table(table);
    mghashtable_finish(&table);
    print_table(table);
}

int main(void)
{
    test_hashtable2();
    return 0;
}

#endif

/*
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
*/
