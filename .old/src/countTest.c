#include <stdio.h>

int main(void)
{
   long num = 1e5;
   long i = 0;
   printf("counting to %ld... ", num);
   while(num--)
   {
      i++;
   }
   printf("done!\n");
}
