#ifndef PRIME_H
#define PRIME_H

#include "types.h"

/*******************************************************************************
  Function Prototypes                       
*******************************************************************************/
long gcd(long a, long b);
/* Returns a^p mod m. */
long exp_mod(long a, long p, long m);
/* Returns the exp of the factor d in a. (a = d^{exp}*e) */
long exp_of(long d, long a);
/* Determines if n is a SPRP in base a */
bool SPRP(long a, long n);
/* Primality of n */
bool prime(long n);

u64 __overflow_mult_mod(u64 a, u64 b, u64 m);


#endif
