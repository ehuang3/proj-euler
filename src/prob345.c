#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
  Defines & Macros                       
*******************************************************************************/
#define M_SIZE 15 //This code only works with fixed sized matrices :(

/*******************************************************************************
  Typedefs                       
*******************************************************************************/
typedef int bool;
typedef int Matrix[][M_SIZE];

/*******************************************************************************
  Prototypes                       
*******************************************************************************/
void print_mat(Matrix m);
/* Updates mask, assuming we are solving left to right */
/* bmsk: bit-mask, using bits to track who applied the mask */
/* mi: index of mask to apply */
void update_mask(Matrix m, Matrix bmsk, int mi, int t_r, int t_c);
/* 0 is first mask, 14 is last mask */
void remove_mask_i(Matrix bmsk, int i);
/* 14 to i is removed */
void remove_mask_toi(Matrix bmsk, int i);
bool is_2x2_optimal(Matrix m, int r1, int c1, int r2, int c2);
int find_sum(Matrix m, size_t msize, Matrix bmsk, int col);

/*******************************************************************************
  Main                       
*******************************************************************************/
int bmask[M_SIZE][M_SIZE];

int main(void)
{
   int M1[5][5] =
   {
      {  7,  53, 183, 439, 863},
      {497, 383, 563,  79, 973},
      {287,  63, 343, 169, 583},
      {627, 343, 773, 959, 943},
      {767, 473, 103, 699, 303}
   };
   
   int M2[15][15] = 
   {
      {  7,  53, 183, 439, 863, 497, 383, 563,  79, 973, 287,  63, 343, 169, 583},
      {627, 343, 773, 959, 943, 767, 473, 103, 699, 303, 957, 703, 583, 639, 913},
      {447, 283, 463,  29,  23, 487, 463, 993, 119, 883, 327, 493, 423, 159, 743},
      {217, 623,   3, 399, 853, 407, 103, 983,  89, 463, 290, 516, 212, 462, 350},
      {960, 376, 682, 962, 300, 780, 486, 502, 912, 800, 250, 346, 172, 812, 350},
      {870, 456, 192, 162, 593, 473, 915,  45, 989, 873, 823, 965, 425, 329, 803},
      {973, 965, 905, 919, 133, 673, 665, 235, 509, 613, 673, 815, 165, 992, 326},
      {322, 148, 972, 962, 286, 255, 941, 541, 265, 323, 925, 281, 601,  95, 973},
      {445, 721,  11, 525, 473,  65, 511, 164, 138, 672,  18, 428, 154, 448, 848},
      {414, 456, 310, 312, 798, 104, 566, 520, 302, 248, 694, 976, 430, 392, 198},
      {184, 829, 373, 181, 631, 101, 969, 613, 840, 740, 778, 458, 284, 760, 390},
      {821, 461, 843, 513,  17, 901, 711, 993, 293, 157, 274,  94, 192, 156, 574},
      { 34, 124,   4, 878, 450, 476, 712, 914, 838, 669, 875, 299, 823, 329, 699},
      {815, 559, 813, 459, 522, 788, 168, 586, 966, 232, 308, 833, 251, 631, 107},
      {813, 883, 451, 509, 615,  77, 281, 613, 459, 205, 380, 274, 302,  35, 805}
   };
   
   int* matrix = (int*)M2;
   
   int r=1, c=1;
   
   //print_mat(M1);
   
   //update_mask(M1, bmask, 0, 14, 0);
   //update_mask(M1, bmask, 1, 3, 1);
   
   //print_mat(bmask);
   
   int sum = find_sum(M2, M_SIZE, bmask, 0);
   printf("sum = %d\n", sum);
}

/*******************************************************************************
  Implementation                       
*******************************************************************************/
int find_sum(Matrix m, size_t msize, Matrix bmsk, int col)
{
   int sum = 0, max = 0;
   
   for(int r=0; r<msize; r++)
   {
      if(!bmsk[r][col]) //No mask
      {
         update_mask(m, bmsk, col, r, col);
         
         int s = 0;
         if(col < msize-1)
            s = find_sum(m, msize, bmsk, col+1);
         
         if(s + m[r][col] > max)
            max = s + m[r][col];
         
         remove_mask_i(bmsk, col);
      }
   }
   
   return max;
}

void update_mask(Matrix m, Matrix bmsk, int mi, int t_r, int t_c)
{
   int mb = 1 << mi;
   for(int c=t_c+1; c<M_SIZE; c++)
      for(int r=0; r<M_SIZE; r++)
      {
         if(r==t_r)
         {
            bmsk[r][c] |= mb; //Mask row always
         }
         else if(!is_2x2_optimal(m,t_r,t_c,r,c)) 
         {
            bmsk[r][c] |= mb; //Mask non-optimal
         }
      }
}

void remove_mask_i(Matrix bmsk, int i)
{
   int off = ~(1 << i);
   int num = M_SIZE*M_SIZE;
   
   int* msk = (int*) bmsk;
   while(num--)
   {
      *msk = *msk & off;
      msk++;
   }
}

void remove_mask_toi(Matrix bmsk, int i)
{
   int off = ~(-1 << i);
   int num = M_SIZE*M_SIZE;
   
   int* msk = (int*) bmsk;
   while(num--)
   {
      *msk = *msk & off;
      msk++;
   }
}

bool is_2x2_optimal(Matrix m, int r1, int c1, int r2, int c2)
{
   return m[r1][c1]+m[r2][c2] > m[r1][c2]+m[r2][c1];
}

void print_mat(Matrix m)
{
   printf("Matrix:\n");
   for(int r=0; r<M_SIZE; r++){
      printf("   ");
      for(int c=0; c<M_SIZE; c++){
         printf("%3d ", m[r][c]);
      }
      printf("\n");
   }
}
