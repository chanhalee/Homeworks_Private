#include <stdio.h>


typedef struct ku_pte {
	char eight_bit;
}	t_my_pte;

int main(void)
{
	char a = 128;
	printf("%lu\n", sizeof(t_my_pte));
	printf("%d\n", a);
	printf("%u\n", a);
}