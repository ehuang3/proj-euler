#pragma once
#include <vector>
#include <iostream>

namespace Prime{
	long gcd(long a, long b);
	/* Returns a^p mod m */
	long expMod(long a, long p, long m);
	/* Returns pow in a = d^{pow}e */
	long powOf(long d, long a);
	/* Determines in n is SPRP in base a */
	bool SPRP(long a, long n);
	/* Primality of n */
	bool prime(long n);
	long nextPrime(long n);
	/* Euler totient of n */
	long totient(long n);
	std::vector<long> divisors(long n);
}
