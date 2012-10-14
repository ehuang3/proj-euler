#include <stdio.h>
#include <stdlib.h>
#include "prime.h"


typedef struct p09
{
	u32 prime;
	u32 digits;
	u32 length;
} p09;

void find_primes(u64 num, u32 digits, u32 length);
u64 find_sets(u32 set, u32 len, p09* p);

int p09_compar(const void* e, const void* f);

p09 array[43090];
int main(void)
{
	find_primes(0,0,0);
	qsort(&array[0], 43089, sizeof(p09), p09_compar);
	//for(int i=0; i<43089; i++)
	//{
		//printf("#%d - %d %#x %d\n",i,array[i].prime, array[i].digits, array[i].length);
	//}
	array[43089].length = 100; //
	
	u64 count = find_sets(0,0,&array[0]);
	printf("count = %ld\n", count);
}

u64 find_sets(u32 set, u32 len, p09* p)
{
	if(len == 9)// && set == 0x1FF)
	{
		static u64 cc = 0;
		cc++; //printf("cc = %ld\n", cc);
		return 1;
	}
	
	u64 count = 0;
	
	while(len + p->length <= 9)
	{
		if(!(p->digits&set))
		{
			count += find_sets(set|p->digits, len+p->length, p+1);
		}
		p++;
	}
	
	return count;
}

int p09_compar(const void* e, const void* f)
{
	p09* pe = (p09*) e, *pf = (p09*) f;
	return pe->prime - pf->prime;
}

void find_primes(u64 num, u32 digits, u32 length)
{
	if(prime(num))
	{
		static u32 count = 0;
		//printf("#%d - %ld\n",count,num);
		array[count].prime = num;
		array[count].digits = digits;
		array[count].length = length;
		count++;
	}
	
	//
	u64 nn = num*10;
	
	u64 i = 0;
	while(i++ < 9)
	{
		if((digits>>(i-1))&1)
		{
			//Do nothing
		}
		else
		{
			find_primes(nn+i, digits|(1<<(i-1)), length+1);
		}
	}
}
