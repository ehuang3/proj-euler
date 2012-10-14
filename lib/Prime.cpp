#include "Prime.h"
#include <stdlib.h>

static long miller_rabin[] = {2,3,5,7,11,13,17};

u64 __overflow_mult_mod(u64 a, u64 b, u64 m);

long Prime::gcd(long a, long b)
{
   if(a==0) return b;
   if(b==0) return a;
   long r;
   if(a>b) { r=b; b=a; a=r; }
   while( (r=b%a)!=0 ) { b=a; a=r; }
   return labs(a);
}

long Prime::exp_mod(long a, long p, long m)
{
   long ans=1;
   while(p)
   {
      if(p&1) { ans = __overflow_mult_mod(a, ans, m); }
      p /= 2;
      a = __overflow_mult_mod(a,a,m);
   }
   return ans;
}

long Prime::exp_of(long d, long a)
{
   long exp=0;
   if(a) { while(a%d==0) { a=a/d; exp++; } }
   return exp;
}

bool Prime::SPRP(long a, long n)
{
   long d=n-1;
   long s=exp_of(2,d);
   d=d>>s;
   long t=exp_mod(a,d,n);
   if(t==1||t==n-1)
      return true;
   long i=1;
   while(i<s)
   {
      t=exp_mod(t,2,n);
      if(t==n-1) { return true; }
      ++i;
   }
   return false;
}

bool Prime::prime(long n)
{
   if(n <= 1)
      return false;
   for(int i=0; i<7; ++i)
   {
      if(miller_rabin[i]==n)
         return true;
      if(!SPRP(miller_rabin[i],n))
         return false;
   }
   return true;
}

long Prime::next_prime(long n)
{
   if(n <= 1)
      return 2;
   if(n == 2)
      return 3;
   if(n%2==0) n += 1;
   else       n += 2;
   while(!prime(n)) { n+=2; }
   return n;
}

long Prime::phi(long n)
{
   using namespace std;
   vector<factor> v = factors(n);
   for(auto f=v.begin(); f!=v.end(); ++f)
   {
      n -= n/f->p;
   }
   return n;
}

std::vector<factor> Prime::factors(long n)
{
   using namespace std;
   vector<factor> v;
   if(n==1) return v;
   long p=2;
   while(n!=1)
   {
      long e = exp_of(p,n);
      if(e)
      {
        v.push_back(factor(p,e));
        while(n%p==0) { n /= p; }
      }
      if(prime(n)) { v.push_back(factor(n,1)); return v; }
      p = next_prime(p);
   }
   return v;
}


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
