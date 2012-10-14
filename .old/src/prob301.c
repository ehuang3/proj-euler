#include <stdio.h>
#include <stdlib.h>

int main(void)
{
   unsigned long n = 1;
   long c = 0;
   while(n <= 0x40000000) 
   {
      if(!(n^(2*n)^(3*n)))
      {
         //printf("n: %ld\n",n);
         //printf("nimsum: %lx\n", n^(2*n)^(3*n));
         c++;
      }
      n++;
   }
   printf("n: %ld\n", n);
   printf("answer: %ld\n", c);
}
