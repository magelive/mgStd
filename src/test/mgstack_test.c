#include <stdio.h>
#include <stdlib.h>

#include "mgstack.h"
#include "mgtime.h"

int comp(void *data1, void *data2)
{
	int a = **(int **)data1;
	int b = **(int **)data2;
	return b-a;
}

int main()
{
	int i ;
	mg_stack *stack = mg_stack_init(0, comp);
	for (i = 0; i < 10; i++) {
		int *num = (int *)malloc(sizeof(int));
		*num = mg_get_rand_num(0, 100);
		mg_stack_push(stack, num);
	}
	for ( i = 0; i < 10; i++) {
		printf(" %d ", *(int *)stack->data[i]);
	}
	printf("\n");
	mg_stack_sort(stack);

	for ( i = 0; i < 10; i++) {
		printf(" %d ", *(int *)stack->data[i]);
	}
	printf("\n");

	for (i = 0; i < 10; i++) {
		int *n = mg_stack_pop(stack);
		printf(" %d ", *n);
		free(n);
	}
	printf("\n");
	return 0;
}
