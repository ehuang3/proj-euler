#include <stdio.h>
#include <stdlib.h>
#include "big_integer.h"


u32 sumDigits(BI* b);
u32 find_exps(u32 sum, u32 max_exp);

#define MAX 1000
#define MAX_EXP 100
BI* array[MAX];
int main(void)
{
	for(int i=0; i<MAX; i++)
	{
		array[i] = BI_Create(1,1);
	}
	u32 count = 0;
	for(u32 i=2; i<100; i++)
	{
		count += find_exps(i,MAX_EXP);
	}
	qsort(array, count, sizeof(BI*), BI_ComparePtr);
	for(int i=0; i<count; i++)
	{
		printf("#%d- ",i+1); BI_PrintDeci(array[i]);
	}
}

u32 sumDigits(BI* b)
{
	BI* t = BI_Create(1,1); BI_Copy(t,b);
	u32 sum = 0;
	while(!BI_isZero(t))
	{
		sum += BI_DivOneWord(t,t,10);
	}
	BI_Free(t);
	return sum;
}

u32 find_exps(u32 sum, u32 max_exp)
{
	BI* n = BI_Create(sum,1);
	u32 count = 0;
	BI_MultOneWord(n,n,sum);
	u32 exp = 2;
	while(exp<max_exp)
	{
		if(sumDigits(n)==sum)
		{
			static u32 ii = 0;
			BI_Copy(array[ii++],n);
			count++;
		}
		BI_MultOneWord(n,n,sum);
		exp++;
	}
	BI_Free(n);
	return count;
}
