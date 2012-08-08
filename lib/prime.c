#include "prime.h"
#include <stdio.h>

/*******************************************************************************
  Static Global                       
*******************************************************************************/
static long miller_rabin[] = {2,3,5,7,11,13,17};

/****************************************************************************
  Internal functions                       
****************************************************************************/
/* To solve the problem of overflow during a*b %m */
/* Rabin-Miller only works up to 341550071728321 ~= 3.4 e14 */
/* likewise this function will probably only work up to that limit */
u64 __overflow_mult_mod(u64 a, u64 b, u64 m)
{
	// Common case
	if(a <= ((u32)-1) && b <= ((u32)-1))
		return (a*b)%m;
	
	// 3.4 e14 ~= 2^48 + 2^...
	// ==> max sqr is < 2^98, so [0.97]
	// ==> addition mod m cannot overflow
	
	// Low bits of a*b%m
	u64 ans = (a*b)%m;
	
	// High bits [64.127] of a*b
	u64 high = (a&0xFFFFFFFF)*(b&0xFFFFFFFF)>>32;
	high += (a>>32)*(b&0xFFFFFFFF);
	high += (a&0xFFFFFFFF)*(b>>32);
	high = high >> 32;
	high += (a>>32)*(b>>32);
	high = high%m; // Necessary?
	
	// 2^64 % m
	u64 factor = ((u64)-1)%m;
	factor = (factor+1)%m;
	
	while(high) // while overflow..
	{
		// Low bits of high*factor%m
		ans = (ans+(high*factor)%m)%m;
		
		// High bits of high*factor;
		u64 buf = (high&0xFFFFFFFF)*(factor&0xFFFFFFFF)>>32;
		buf += (high>>32)*(factor&0xFFFFFFFF);
		buf += (high&0xFFFFFFFF)*(factor>>32);
		buf = buf >> 32;
		buf += (high>>32)*(factor>>32);
		
		// Store higher bits
		high = buf;
	}
	
	return ans;
}

/*******************************************************************************
  Implementation                       
*******************************************************************************/
long gcd(long a, long b)
{
   if(a==0)
      return b;
   if(b==0)
      return a;
   
   long r;
   if(a > b)
   {
      r = b;
      b = a;
      a = r;
   }
   while( (r=b%a) != 0 )
   {
      b = a;
      a = r;
   }
   return a > 0 ? a : -a;
}

long exp_mod(long a, long p, long m)
{
   long ans = 1;
   while(p)
   {
      if(p&1)
      {
      	//printf("a=%ld * ans=%ld mod m",a,ans);
         ans = __overflow_mult_mod(a, ans, m);
         //printf(" = %ld\n\n",ans);
         p--;
      }
      p = p/2;
      //printf("%ld^2 mod m", a);
      a = __overflow_mult_mod(a,a,m);
      //printf(" = %ld\n", a);
   }
   //printf("\n");
   return ans;
}

long exp_of(long d, long a)
{
   long exp = 0;
   if(a)
   {
      while(a%d == 0)
      {
         a = a/d;
         exp ++;
      }
	} 
   return exp;
}

bool SPRP(long a, long n)
{
   long d = n-1;
   long s = exp_of(2, d);
   d = d >> s;
   long t = exp_mod(a,d,n);
   if(t == 1 || t == n-1)
      return true;
      
   long i = 1;
   while(i < s)
   {
      t = exp_mod(t,2,n);
      
      if(t == n-1)
         return true;
         
      i++;
   }
   return false;
}

bool prime(long n)
{
   if(n <= 1)
      return false;
   for(int i=0; i<7; i++)
   {
      if(miller_rabin[i] == n)
         return true;
      if(!SPRP(miller_rabin[i], n))
         return false;
   }
   return true;
}
