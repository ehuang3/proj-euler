#include "types.h"
#include "prime.h"
#include <stdio.h>

/*******************************************************************************
  Prototype                       
*******************************************************************************/
long sum_SRTHP(long hsd, int ndgt, int max);
long div_dsum(long n);

/*******************************************************************************
  Main                       
*******************************************************************************/
int main(void)
{
	int digits = 5;
	printf("SUM = %ld\n", sum_SRTHP(0,0,14));
	printf("MAXLONG = %ld\n", 0x7FFFFFFFFFFFFFFF);
}

/*******************************************************************************
  Implementation                       
*******************************************************************************/
long sum_SRTHP(long hsd, int ndgt, int max)
{
	if(ndgt == max)
		return 0;
		
	//printf("HARSHAD = %14ld\n",hsd);
	
	long sum = 0;
	long nhsd = hsd*10;
	
	if(prime(div_dsum(hsd)))
	{
		int i = 1;
		while(i<10)
		{
			if(prime(nhsd+i))
			{
				sum += nhsd+i;
				printf("SRTHP = %14ld\n",nhsd+i);
			}
			i +=2;
		}
	}
	
	int i = 10;
	while(i--)
	{
		if(div_dsum(nhsd+i))
			sum += sum_SRTHP(nhsd+i, ndgt+1, max);
	}
	
	return sum;
}

long div_dsum(long n)
{
	long d = n;
	int sum = 0;
	while(d)
	{
		sum += d%10;
		d /= 10;
	}
	if(n)
		return n%sum == 0 ? n/sum : 0;
	return 0;
}
