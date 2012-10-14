#include <stdio.h>

#define MAX_WIDTH 128
#define MAX_HEIGHT 128

int array[MAX_WIDTH][MAX_HEIGHT];

int abs(int a);
int value(int r, int c);
void calc_rects(int r, int c);

int main (int argc, char *argv[])
{
   for(int r = 1; r <= MAX_HEIGHT; r++)
   {
      for(int c = 1; c <= MAX_WIDTH; c++)
      {
         calc_rects(r,c);
      }
   }
   
   printf("rects for 3x2 = %d\n",value(64,64));
   
   //Find closest to 2 million
   int ROW;
   int COL;
   int delta = 0x7FFFFFFF;
   
   for(int r = 1; r <= MAX_HEIGHT; r++)
   {
      for(int c = 1; c <= MAX_WIDTH; c++)
      {
         if( abs(2000000-value(r,c)) < delta )
         {
            ROW = r;
            COL = c;
            delta = abs(2000000-value(r,c));
         }
      }
   }
   
   printf("%dx%d = %d\n", ROW, COL, value(ROW,COL));
   
   printf("%d\n", ROW*COL);
   
   return 0;
}

int abs(int a)
{
   return a > 0 ? a : -a;
}

/* Pretend array has starting indicies at (1,1) */
int value(int r, int c)
{
   return array[r-1][c-1];
}

void set_value(int v, int r, int c)
{
   array[r-1][c-1] = v;
}

/* Calculates the number of rects in a rxc rect, assuming the value is known for
   the (r-1)xc rect */
void calc_rects(int r, int c)
{
   if(r == 1 && c == 1)
   {
      //Row is 1, Col is 1
      set_value(1,r,c);
      return;
   }
   
   int rects;
   int sizes;
   int num;
   int max;
   
   //Switch
   if(r==1)
   {
      rects = 0;
      sizes = r;
      num = c;
      max = c;
   }
   else if(c==1)
   {
      rects = 0;
      sizes = c;
      num = r;
      max = r;
   }
   else if(value(r-1,c))
   {
      rects = value(r-1,c);
      sizes = r;
      num = c;
      max = c;
   }
   else if(value(r,c-1))
   {
      rects = value(r,c-1);
      sizes = c;
      num = r;
      max = r;
   }
   
   //New rects computation
   do
   {
      rects += (max-num+1)*sizes;
   } while (--num);
   
   set_value(rects, r, c);
}


