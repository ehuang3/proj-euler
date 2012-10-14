#include "prime.h"
#include <stdio.h>

int main(void)
{
//	long i = 3;
//	while(i < 1e5)
//	{
//		if(prime(i))
//			printf("%ld\n",i);
//		i += 2;
//	}
	
	printf("Prime Test: \n\n");
	
	printf("Is 999999999989 prime? \n");
	if(prime(999999999989))
		printf("yes\n");
	else
		printf("no\n");
	printf("\n");
	
	long a = 0x100000000;
	long b = 0x100000000;
	long m = 341550071728321;
	printf("Testing __overflow_mult_mod: \n");
	printf("__overflow_mult_mod(%lx,%lx,%lx) = %lx\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("__overflow_mult_mod(%ld,%ld,%ld) = %ld\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("\n");
	
	a = m-1;
	b = m-1;
	printf("__overflow_mult_mod(%lx,%lx,%lx) = %lx\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("__overflow_mult_mod(%ld,%ld,%ld) = %ld\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("\n");
	
	a = 73912465800;
	b = 73912465800;
	m = 999999999989;
	printf("__overflow_mult_mod(%lx,%lx,%lx) = %lx\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("__overflow_mult_mod(%ld,%ld,%ld) = %ld\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("\n");
	
	a = 518540591336;
	b = 222330515430;
	m = 999999999989;
	printf("__overflow_mult_mod(%lx,%lx,%lx) = %lx\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("__overflow_mult_mod(%ld,%ld,%ld) = %ld\n",a,b,m, __overflow_mult_mod(a,b,m));
	printf("\n");
}
