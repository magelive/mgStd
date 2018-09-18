#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "mgstack.h"

#ifndef DEFAULT_NUM_MAX
#define DEFAULT_NUM_MAX 64
#endif

//#define USE_BUBBLE_SORT
#define USE_FAST_SORT
#ifdef USE_FAST_SORT
#include "mgfastsort.h"
static void mg_stack_swap(void *list, int i, int j)
{
	if (!list) 
		return;
	void **data = (void **)list;
	void *tmp = NULL;
	tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
	return;
}
#endif

static int _stack_ptr_comp(void *data1, void *data2)
{
	return (data2-data1);
}

mg_stack *mg_stack_init(int data_max, stack_comp comp)
{
	mg_stack *stack = (mg_stack *)malloc(sizeof(mg_stack));
	if (!stack) {
		return NULL;
	}
	stack->data_num = 0;
	stack->sorted = 0;
	stack->limit_num = 1;
	if (data_max == 0) {
		stack->limit_num = 0;
		data_max = DEFAULT_NUM_MAX; 
	}
	stack->data = (void **)malloc(sizeof(void *) * data_max);
	if (!stack->data) {
		free(stack);
		return NULL;
	}
	bzero(stack->data, sizeof(void *)*data_max);
	stack->alloc_num = data_max;
	stack->comp = _stack_ptr_comp;
	if (comp) {
		stack->comp = comp;
	}
	return stack;
}

int mg_stack_push(mg_stack *stack, void *data)
{
	if (!stack || !data) {
		return -1;
	}
	
	if (stack->data_num == stack->alloc_num) {
		if (stack->limit_num) {
			return -2;
		}
		int num = stack->alloc_num * 2;
		void *ndata = realloc(stack->data, num*(sizeof(void *)));
		if (!ndata) {
			return -3;
		}
		stack->data = (void **)ndata;
		stack->alloc_num = num;
	}
	stack->data[stack->data_num] = data;
	stack->data_num++;
	stack->sorted = 0;
	return 0;
}

void* mg_stack_pop(mg_stack *stack)
{
	if (!stack || stack->data_num <= 0) {
		return NULL;
	}
	stack->data_num--;
	void *data = stack->data[stack->data_num];
	return data;
}

void mg_stack_sort(mg_stack *stack)
{
	if (!stack || stack->data_num <= 1 || !stack->comp || stack->sorted) {
		return;
	}
#ifdef USE_BUBBLE_SORT
	void *tmp = NULL;
	int i = 0, j = 0;
	for ( i = 0; i < stack->data_num; i++) {
		for ( j = i+1; j < stack->data_num; j++) {
			if (stack->comp(stack->data[i], stack->data[j])) {
				tmp = stack->data[i];
				stack->data[i] = stack->data[j];
				stack->data[j] = tmp;
				tmp = NULL;
			}
		}
	}
#endif
#ifdef USE_FAST_SORT
	mgfastsort(stack->comp, mg_stack_swap, (void *)(stack->data), 0, stack->data_num-1, sizeof(void *));
#endif
	stack->sorted = 1;
	return;
}
