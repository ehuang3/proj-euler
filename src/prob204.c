#include <stdio.h>
#include "prime.h"


long primes[100];

long find_hamming(long,long,long*,long);

long find_hamming(long n, long max_prime, long* prime, long max)
{
	if(n > max)
		return 0;
	
	long count = 1;
	
	while(*prime < max_prime)
	{
		count += find_hamming(n*(*prime), max_prime, prime, max);
		prime++;
	}
	
	return count;
}

int main(void)
{
	long count = 0;
	
	primes[0] = 2;
	for(int i=1; i<100; i++)
	{
		primes[i] = next_prime(primes[i-1]);
	}
	
	count = find_hamming(1, 100, &primes[0], 1e9);
	
	printf("count = %ld\n", count);

}
