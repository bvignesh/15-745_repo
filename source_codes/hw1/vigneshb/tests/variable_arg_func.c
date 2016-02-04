#include <stdio.h>
#include <stdlib.h>

int var_arg_func(int a, int b, ...)
{
	printf("I am just a fancy function that takes multiple args and does nothing about it\n");
	return 0;
}

int func2(int x, int y)
{
	int c = x + y;
	return c;
}

int ARR[20] = {0}; 

int func_with_array_arg(int* arr)
{
	int array[18];
	for (int i = 0; i < 18; i++)
		array[i] = arr[i];
	return array[5];
}

int func_that_uses_all()
{

	int a = var_arg_func(1,2,3,4,5);
	int b = func2(3,4);
	int c = func_with_array_arg(ARR);
	return 0;
}
