#ifndef _MGSTD_STACK_H
#define _MGSTD_STACK_H

#define DEFAULT_NUM_MAX 64

typedef int(*stack_comp)(void *data1, void *data2);

typedef struct mg_stack {
	int data_num;
	int alloc_num;
	int sorted:1;
	int limit_num:1;
	void **data;
	stack_comp comp;
}mg_stack;

mg_stack *mg_stack_init(int data_max, stack_comp comp);

int mg_stack_push(mg_stack *stack, void *data);

void* mg_stack_pop(mg_stack *stack);

void mg_stack_sort(mg_stack *stack);

#endif
