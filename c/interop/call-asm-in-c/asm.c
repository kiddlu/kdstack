#include <stdio.h>

int func(int a, int b)
{
	return a - b;
}

int main()
{
	int a = 12, b = 5;
	int res;
 	__asm__ ( 
		push b;
		push a;
		call func;    //return in %eax
		mov res, eax;
	);
	printf("%d\n", res);
	return 0;
}