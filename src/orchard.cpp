#include <iostream>
#include "Prime.h"

using namespace std;

int main()
{
   cout <<       "\tProblem 351\n";
   cout << "===========================" << endl;
   long max = 100000000;
   int* phi = new int[max+1];
   long p = next_prime(1);
   for(int i=0; i<max+1; i++)
      phi[i] = 1;
   while(p<max+1)
   {
      long pn = p;
      long prev = 1;
      while(pn<max+1)
      {
         long i = 1;
         while(i*pn<max+1)
         {
            if(i%p != 0)
               phi[i*pn] *= (pn-prev);
            ++i;
         }
         prev = pn;
         pn *= p;
      }
      p = next_prime(p);
   }
   long o = 1;
   for(long n = 2; n <= max; ++n)
   {
      if(n%2 == 1)
      {
         //long fi = Prime::phi(n);
         long fi = phi[n];
         o += n-fi;
         if(2*n <= max)
            o += 2*n-fi;
      }
      else if(2*n <= max)
      {
         //long fi = Prime::phi(n);
         long fi = phi[n];
         o += 2*n-2*fi;
      }
      //cout << n << endl;
   }
   cout << "ans = " << o*6 << endl;
   delete phi;
}
