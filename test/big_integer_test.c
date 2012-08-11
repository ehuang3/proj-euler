#include <stdio.h>
#include "big_integer.h"

int main(void)
{
	printf("BigInteger Test:\n\n");
	
	printf("Creating new big int\n");
	BI* b = BIcreate(-1);
	BIprint(b);
	printf("\n");
	
	printf("Adding to self, checking for overflow correctness\n");
	BIadd(b,b,b);
	BIprint(b);
	printf("\n");
	
	printf("Add to self 63 more times\n");
	for(int i=0; i<63; i++)
	{
		BIadd(b,b,b);
		BIprint(b);
	}
	printf("\n");
	
	printf("Test mem overflow\n");
	BIprint(b);
	printf("after adding!!!!\n");
	BIadd(b,b,b);
	BIprint(b);
	printf("\n");
	
	printf("Free BI b, prep mult\n");
	BIfree(b);
	b = BIcreate(0xFFFFFFFF);
	BIprint(b);
	printf("\n");
	
	BImult(b,b,b);
	printf("Just did mult: \n");
	BIprint(b);
	printf("\n");
	
	printf("what it looks like with u64\n");
	printf("%#x^2 = %#lx\n\n",0xFFFFFFFF, (u64)0xFFFFFFFF * (u64)0xFFFFFFFF);
	
	printf("Testing mult: \n");
	BI* a = BIcreate(19283124123123); //19283124123123
	BI* x = BIcreate(12345123124312412); //12345123124312412
	BI* dst = BIcreate(0);
	BImult(dst,a,x);
	BIprint(dst);
	
	printf("\n");
	
	printf("Do it more:\n");
	BImult(dst,dst,x);
	BIprint(dst);
	BImult(dst,dst,x);
	BIprint(dst);
	
}
