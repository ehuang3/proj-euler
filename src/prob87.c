#include <stdio.h>
#include "prime.h"

char numbers[50000000];
int primes[1000];

int find_sum(int sum, int power);
int _exp(int n, int power);

int main(void)
{
	primes[0] = 2;
	for(int i=1; i<1000; i++)
	{
		primes[i] = next_prime(primes[i-1]);
		//printf("%d\n",primes[i]);
	}
	
	int n_count = find_sum(0, 2);
	printf("naive count = %d\n",n_count);
	
	int b_count = 0;
	for(int i=0; i<50000000; i++)
	{
		if(numbers[i])
			b_count ++;
	}
	printf("better count = %d\n",b_count);
}

int find_sum(int sum, int power)
{
	if(sum > 50000000)
		return 0;
	
	if(power > 4) //past the last power
	{
		//printf("sum = %d\n",sum);
		numbers[sum-1] |= 1;
		return 1;
	}
	
	int count = 0;
	
	int r_count = 1;
	int* prime = &primes[0];
	
	while(r_count)
	{
		r_count = find_sum(sum+_exp(*prime, power), power+1);
		
		count += r_count;
		prime ++;
	}
	
	return count;
}

int _exp(int n, int power)
{
	int ans = 1;
	while(power--)
	{
		ans *= n;
	}
	return ans;
}
