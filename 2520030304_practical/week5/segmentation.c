#include<stdio.h>

int main() {
	int  *ptr = NULL;
	int x = 10;
	ptr = &x;
	printf("x = %d" ,x);
	printf("address of ptr = %p\n", (void *)&ptr);
	printf("value of ptr = %p\n", (void *)ptr);
	printf("value at ptr = %d", *ptr);
	printf("address of ptr = %p\n", (void *)&ptr);
	return 0;
}
