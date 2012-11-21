#pragma once

#include <vector>

namespace Prime
{
   typedef unsigned long u64;
   typedef unsigned int  u32;

   struct factor
   {
      long p, e;
      factor() : p(0), e(0) {}
      factor(long pp, long ee) : p(pp), e(ee) {}
   };

   // GCD of a and b.
   long gcd(long a, long b);
   // LCM of a and b.
   long lcm(long a, long b);
   // Returns a^p mod m.
   long exp_mod(long a, long p, long m);
   // Returns the exponent of the factor d in a.
   // a = d^{exp}*e
   long exp_of(long d, long a);
   // True if n is SPRP in base a.
   bool SPRP(long a, long n);
   // True if n is prime.
   bool prime(long n);
   // Next prime after n.
   long next_prime(long n);
   // Euler totient of n.
   long phi(long n);
   // Factors of n.
   std::vector<factor> factors(long n);
}
using namespace Prime;
