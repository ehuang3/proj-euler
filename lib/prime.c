#include "prime.h"
#include <stdio.h>

/*******************************************************************************
  Static Global                       
*******************************************************************************/
static long miller_rabin[] = {2,3,5,7,11,13,17};

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
         ans = (ans*a)%m;
         p--;
      }
      p = p/2;
      a = (a*a)%m;
   }
   return ans;
}

int exp_of(long d, long a)
{
   int exp = 0;
   if(a)
      while(a%d == 0)
      {
         a = a/d;
         exp ++;
      }
   return exp;
}

bool SPRP(long a, long n)
{
   long d = n-1;
   int s = exp_of(2, d);
   int i = s;
   while(i--)
   {
      d = d/2;
   }
   long t = exp_mod(a,d,n);
   if(t == 1 || t == n-1)
      return true;
   i = s;
   while(i-- > 1)
   {
      t = exp_mod(t,2,n);
      if(t == n-1)
         return true;
   }
   return false;
}

bool prime(long n)
{
   if(n == 1)
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



