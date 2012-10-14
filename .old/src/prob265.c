#include <stdio.h>

typedef unsigned int u32;
typedef unsigned long u64;


int is_circ(u32 s);
u32 circ_shift(u32 x, u32 n);

int main()
{
	u32 max = 0x07FFFFFF;
	u32 x = 0;
	u64 sum = 0;
	while(x < max)
	{
		if(is_circ(x))
		{
			//printf("x = %.8x\n",x);
			sum += x;
		}
		x++;
	}
	printf("sum = %ld\n",sum);
}

int is_circ(u32 x)
{
	u32 mask = 0x1F;
	u32 hit = 0;
	
	u32 i = 32;
	while(i--)
	{
		hit ^= 1<<(mask&x);
		x = circ_shift(x,1);
	}
	if(hit == -1)
		return 1;
	else
		return 0;
}

u32 circ_shift(u32 x, u32 n)
{
	n = n%32;
	return (x>>n)|(x<<(32-n));
}
