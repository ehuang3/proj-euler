#include <stdio.h>


int set[12001];
int N[15000];

int sum_set(int* set, size_t k);
int prod_set(int* set, size_t k);
int find_N(int* set, size_t k);
void re_set(int* set, size_t k);
void print_set(int* set, size_t k);

int main(void)
{
	for(int i=0; i<12001; i++)
	{
		set[i] = 1;
	}
	
	for(int k=3; k <=12000; k++)
	{
		int n = find_N(&set[0], k);
		//print_set(&set[0], k);
		
		//printf("N = %d k = %d\n\n",n,k);
		
		//if(n > 15000) printf("out of bounds array \n");
		
		N[n-1] |= 1;
		
		re_set(&set[0], k);
	}
	
	long sum = 0;
	for(int i=0; i<15000; i++)
	{
		if(N[i])
			sum += i+1;
	}
	
	printf("sum = %ld\n", sum);
}

void print_set(int* set, size_t k)
{
	for(int i=0; i < k ; i++)
	{
		printf("%d ",set[i]);
	}
	printf("\n");
}

void re_set(int* set, size_t k)
{
	while(k--)
	{
		set[k] = 1;
	}
}

int sum_set(int* set, size_t k)
{
	int sum = 0;
	while(k--)
	{
		if(set[k] == 1)
			return sum + k + 1;
		sum += set[k];
	}
	return sum;
}

int prod_set(int* set, size_t k)
{
	int prod = 1;
	while(k--)
	{
		if(set[k] == 1)
			return prod;
		prod *= set[k];
	}
	return prod;
}

void set_set(int* set, int start, int end, int value)
{
	for(int i=start; i<end; i++)
	{
		set[i] = value;
	}
}


int find_N(int* set, size_t k)
{
	int ind = k-1;
	set[ind] = 1;
	set[ind-1] = 2;
	
	int prod = prod_set(set, k), sum = sum_set(set, k);
	
	int min = 0x3FFFFFFF;
	
	int min_pp = 0;
	int max_ii = k-2;
	int ii = k-1;
	
	while( min_pp < min )
	{
		
		set[ii] ++;
		
		prod = prod_set(set,k);
		sum = sum_set(set,k);
		
		while(prod > sum)
		{
			// Move to next valid set
			ii--;
			set[ii]++;
			set_set(set, ii, k, set[ii]);
			
			prod = prod_set(set,k);
			sum = sum_set(set,k);
			
			if(ii < max_ii)
			{
				min_pp = prod_set(set,k);
				max_ii = ii;
			}
			
			if(min_pp >= min)
			{
				break;
			}
		}
		
		//print_set(set,k);
		
		ii = k-1;
		
		if(prod == sum && prod < min)
			min = prod;
		
	}
	
	return min;
}










