#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int valid_box(u64 blue, u64 red);
long double invsqrt2 = 0.70710678118654752440;
u64 mult_high(u64 a, u64 b);

int main(void)
{
	int max = 1000;
	for(long b=1; b < max; b++)
	for(long r=1; r < max; r++)
	{
		if(valid_box(b,r))
		{
			double bb = b, rr = r;
			printf("box: b= %ld r= %ld\n",b,r);
			printf("ratio: r/b= %.5f b/t= %.5f r/t= %.5f\n",
						rr/bb, bb/(bb+rr), rr/(bb+rr));
		}
	}
	printf("\n");
	
	printf("1/sqrt(2) = %.18Lf\n",invsqrt2);
	
	printf("1e12/sqrt(2) = %Lf\n",1.1123e12*invsqrt2);
	printf("1e12/sqrt(2) = %ld\n", (u64) (1.1123e12*invsqrt2));
	
	u64 am2 = 1;
	u64 am1 = 3;
	u64 a = 6*am1 - am2 - 2;
	
	while( a < 1e12)
	{
		am2 = am1;
		am1 = a;
		a = 6*am1 - am2 - 2;
		printf("a = %ld\n", a);
	}
	
	
	u64 t = 1;
	while(1)
	{
		u64 blue = t*invsqrt2;
		for(int i=-2; i<=2; i++)
		{
			//printf("t = %ld b = %ld\n", t, blue);
			if(valid_box(blue+i, t))
			{
				printf("blue = %ld total = %ld\n", blue+i, t);
				//exit(0);
			}
		}
		t++;
	}
}

int valid_box(u64 blue, u64 total)
{
	u64 b_high = mult_high(blue*2, blue-1);
	u64 b_low = 2*(blue)*(blue-1);
	
	u64 t_high = mult_high(total,total-1);
	u64 t_low = (total)*(total-1);
	
	return b_high == t_high && b_low == t_low;
}

#define HIGH ((u64)-1<<32)
#define LOW ((u64)-1>>32)
u64 mult_high(u64 a, u64 b)
{
	u64 high = (a&LOW)*(b&LOW)>>32;
	high += (a>>32)*(b&LOW);
	high += (a&LOW)*(b>>32);
	high = high>>32;
	high += (a>>32)*(b>>32);
	
	return high;
}
