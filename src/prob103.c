#include <stdio.h>
#include <stdlib.h>

typedef int bool;

bool check_subset_sum(int* set, size_t num);
bool check_subset_ineq(int* set, size_t num);
int sum(int* set, size_t num);
void print_set(int* set, size_t num);

int main(void)
{
   
   int set[7] = {20, 31, 38, 39, 40, 42, 45};
   printf("sums distinct = %d\n", check_subset_sum(set,7));
   printf("prop ii = %d\n", check_subset_ineq(set,7));
   printf("sum = %d\n", sum(set,7));
   
   int first = 1;
   
   for(int a = 30; a > 8; a--){
   for(int b = 50; b > a; b--){
   for(int c = 51; c > b; c--){
   for(int d = 52; d > c; d--){
   for(int e = 53; e > d; e--){
   for(int f = 54; f > e; f--){
   for(int g = 55; g > f; g--)
   {
      set[0] = a;
      set[1] = b;
      set[2] = c;
      set[3] = d;
      set[4] = e;
      set[5] = f;
      set[6] = g;
      
      if(check_subset_ineq(set,7))
      {
         if(check_subset_sum(set,7))
         {
            if(sum(set,7) <= 255)
               print_set(set,7);
         }
      }
   }
   }}}}}}
   
   return 0;
}

void print_set(int* set, size_t num)
{
   int i = 0;
   while(i < num)
   {
      printf("%d ", set[i]);
      i++;
   }
   printf("= %d\n",sum(set,num));
}

int sum(int* set, size_t num)
{
   int sum = 0;
   while(num--)
   {
      sum += set[num];
   }
   return sum;
}

bool check_subset_sum(int* set, size_t num)
{
   //Create sum array
   int n = num;
   int s = 0;
   while(n--)
   {
      s += set[n];
   }
   int* array = malloc(s*4);
   while(s--)
   {
      array[s] = 0;
   }
   
   //iterate over all subsets
   int perm = 0;
   int max = 1<<num;
   while(++perm < max)
   {
      int sum = 0;
      int ele = num;
      while(ele--)
      {
         if((1<<ele)&perm)
         {
            sum += set[ele];
         }
      }
      if(array[sum-1])
      {
         free(array);
         return 0;
      }
      array[sum-1]++;
   }
   
   free(array);
   return 1;
}

/* Assume set is sorted */
bool check_subset_ineq(int* set, size_t num)
{
   int n = (num-1)/2;
   int i=1;
   
   int front = set[0];
   int back = 0;
   while(n--)
   {
      front += set[i];
      back += set[num-i];
      
      if(back >= front)
         return 0;
      
      i++;
   }
   
   return 1;
}

