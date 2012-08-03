#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
  Prototypes                       
*******************************************************************************/
/* mwd: max width, minf: min fill, maxf: max fill, hofs: horz offset btwn rects */
long* calc_fills(size_t maxwd, int minf, int maxf, int hofs);

/*******************************************************************************
  Main                       
*******************************************************************************/
int main(void)
{
   //Modified for 114-117
   
   long* reds = calc_fills(50, 2, 4, 0);
   printf("%d = %ld\n",50, reds[50]);
   
   long* greens = calc_fills(50, 3, 3, 0);
   printf("%d = %ld\n",50, greens[50]);
   
   long* blues = calc_fills(50, 4, 4, 0);
   printf("%d = %ld\n",50, blues[50]);
   
   printf("total = %ld\n", reds[50] + greens[50] + blues[50] - 3);
   
   return 0;
}

/*******************************************************************************
  Implementaion                       
*******************************************************************************/
long* calc_fills(size_t maxwd, int minf, int maxf, int hofs)
{
   long* fills = malloc(sizeof(long)*(maxwd+1));
   
   //fills[0] is 1x0 rect
   fills[0] = 0;
   fills[1] = 1;
   
   int i=2;
   while(i <= maxwd)
   {
      //calc no fill case
      fills[i] = fills[i-1];
      
      //calc fill case
      for(int f=minf; (f<= maxf || maxf == -1) && f <= i; f++)
      {
         if(f + hofs >= i)
            fills[i] += 1;
         else
            fills[i] += fills[i-f-hofs];
      }
      
      i++;
   }
   
   return fills;
}
