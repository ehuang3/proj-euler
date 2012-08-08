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
	b->bytes[3] = -1;
	BIprint(b);
	printf("after adding!!!!\n");
	BIadd(b,b,b);
	BIprint(b);
	printf("\n");
	
	printf("Free BI b, prep mult\n");
	BIfree(b);
	b = BIcreate(-1);
}
