#define BI_DEBUG

#include "big_integer.h"
#include <stdio.h>


int main(void)
{
	printf("BigInteger Testing:\n\n");
	
	printf("Testing Creation: \n");
	printf("#1- Creating several big integers:\n");
	BI *a,*b,*c;
	a = BI_Create(-1, 1);
	b = BI_Create(-1, -1);
	c = BI_Create(0,0);
	
	printf("a: "); BI_PrintDeci(a);
	printf("b: "); BI_PrintDeci(b);
	printf("c: "); BI_PrintDeci(c);
	printf("\n");
	
	//DEBUG_ASSERT(1 < 0);
	
	printf("#2- UAdd testing:\n");
	BI* o = BI_Create(0,0);
	BI_UAdd(o,a,b);
	BI_PrintDeci(o);
	
	for(int i=0; i<1000; i++)
		BI_UAdd(o,o,a);
	BI_PrintDeci(o); BI_PrintHex(o);
	
	printf("#END- Free BigInteger(s):\n");
	BI_Free(a);
	BI_Free(b);
	BI_Free(c);
	printf("Free'd\n\n");
}
