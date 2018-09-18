#include <stdio.h>
#include <stdlib.h>
#include "mglist.h"


typedef struct mglist_test
{
    int number;
    MG_LIST mglist;
}MG_LIST_TEST;

#define MGLIST_TEST_LEN 10

int main(int argc, char *argv[])
{
    MG_LIST_TEST mglist_test[MGLIST_TEST_LEN];
    MG_LIST_HEAD(mglist1_head);
    MG_LIST_HEAD(mglist2_head);
    int i = 0;
    for (i = 1;i <= MGLIST_TEST_LEN; i++)
    {
        mglist_test[i-1].number = i;
        if (i % 2 == 0)
        {
           mglist_add(&mglist_test[i-1].mglist, &mglist2_head); 
        }
        else
        {
            mglist_add(&mglist_test[i-1].mglist, &mglist1_head);
        }
    }
    MG_LIST *pos = NULL;i=0;printf("mglist1_head list_entry :\n");
    mglist_for_each(pos, &mglist1_head)
    {
        printf("i = %d, pos->number = %d\n", i++, mglist_entry(pos, MG_LIST_TEST, mglist)->number);
    }
    
    i=0;printf("mglist2_head list_entry:\n");
    mglist_for_each(pos, &mglist2_head)
    {
        printf("i = %d, pos->number = %d\n", i++, mglist_entry(pos, MG_LIST_TEST, mglist)->number);
    }
    
    MG_LIST_TEST t1, t2;
    
    t1.number = 55;
    mglist_add_tail(&t1.mglist, &mglist1_head);
    
    t2.number = 56;
    mglist_replace(&mglist_test[5].mglist, &t2.mglist);
    
    
    MG_LIST_TEST *p = NULL; i = 0; printf("mglist1_head for_entry:\n");
    mglist_for_each_entry(p, &mglist1_head, mglist)
    {
        printf("i = %d, pos->number = %d\n", ++i, p->number);
    }
    
    mglist_del(&mglist_test[3].mglist);
    
    i = 0; printf("mglist2_head for_entry:\n");
    mglist_for_each_entry(p, &mglist2_head, mglist)
    {
        printf("i = %d, pos->number = %d\n", ++i, p->number);
    }

}
