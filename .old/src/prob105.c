#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
  Typedef                       
*******************************************************************************/
typedef unsigned int u32;
typedef int bool;

typedef struct pair
{
   u32 p1;
   u32 p2;
} pair;

typedef struct lnode
{
   void* data;
   struct lnode* prev;
   struct lnode* next;
} lnode;

/* Linked list */
typedef struct llist
{
   lnode* head;
} llist;

/*******************************************************************************
  Prototype                       
*******************************************************************************/
llist* create_list();
lnode* create_node(void* data);
void push_front(llist* list, void* data);
int count(llist* list);

int calc_pairs(llist* list, size_t num);
u32 count_bits(u32 x);
u32 high_bit(u32 x);
u32 low_bit(u32 x);
u32 bit_compare(u32 A, u32 B);

int parse_set(char* line, int* set);

bool check_subset_sum(int* set, size_t num);
bool check_subset_ineq(int* set, size_t num);
int set_sum(int* set, size_t num);
void print_set(int* set, size_t num);

void init_pairs(llist* pairs);
bool check_sum_ineq(int* set, llist* pairs);
int calc_sum(u32 perm, int* set);

int compare_int(const void* a, const void* b);

/*******************************************************************************
  Main                       
*******************************************************************************/
llist pairs[12];
int set[12];

int main(void)
{
   init_pairs(pairs);
   int sum = 0;
   
   FILE* fp;
   char* line = NULL;
   size_t len = 0;
   ssize_t read;
   
   fp = fopen("./sets.txt","r");
   
   while( (read = getline(&line, &len, stdin)) != -1)
   {
      int size = parse_set(line, set);
      
      qsort(set, size, sizeof(int), compare_int);
      
      //if(check_sum_ineq(set, &pairs[size-1]))
      //Well, I'm dumb :P
      if(check_subset_sum(set, size) && check_subset_ineq(set, size))
      {
         print_set(set, 12);
         sum += set_sum(set, size);
      }
      
      printf("sum = %d\n",sum);
   }
   
   printf("sum = %d\n",sum);
}

/*******************************************************************************
  Implementation                       
*******************************************************************************/
int compare_int(const void* a, const void* b)
{
   return *((int*)a) - *((int*)b);
}

int calc_sum(u32 perm, int* set)
{
   int sum = 0;
   while(perm)
   {
      if(perm&1)
         sum += *set;
      perm = perm>>1;
      set++;
   }
   return sum;
}

bool check_sum_ineq(int* set, llist* pairs)
{
   lnode* nd = pairs->head;
   while(nd)
   {
      pair* data = nd->data;
      if(calc_sum(data->p1, set) == calc_sum(data->p2, set))
         return 0;
      nd = nd->next;
   }
   return 1;
}

void init_pairs(llist* pairs)
{
   //Only init 7 - 12
   for(int i=7; i<=12; i++)
   {
      calc_pairs(&pairs[i],i);
   }
}

int parse_set(char* line, int* set)
{
   int* end = &set[11];
   return sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
      end--,end--,end--,end--,end--,end--,end--,end--,end--,end--,end--,end--);
}

/* Fills list, returns number of pairs */
int calc_pairs(llist* list, size_t num)
{
   int max = 1 << num;
   
   u32 p1 = 0;
   u32 p2 = 0;
   
   int pairs = 0;
   
   while( ++p1 < max )
   {
      p2 = p1+1;
      while( ++p2 < max )
      {
         //Disjoint and number of bits are equal
         if(!(p1&p2) && count_bits(p1) == count_bits(p2))
         {
            u32 A = p1;
            u32 B = p2;
            //Ensure A < B
            if(B < A)
            {
               A = p2;
               B = p1;
            }
            
            //Compare if overlap
            if(!bit_compare(A,B))
            {
               pair* pp = malloc(sizeof(pair));
               
               if(pp==0)
                  printf("BROEK!");
               
               pp->p1 = A;
               pp->p2 = B;
               
               push_front(list, pp);
               
               pairs ++;
            }
         }
      }
   }
   
   return pairs;
}

u32 bit_compare(u32 A, u32 B)
{
   //A < B
   if(B < A)
   {
      u32 temp = A;
      A = B;
      B = temp;
   }
   
   //Compare bits
   u32 high = 0; //bits to use
   u32 low = 0;
   
   while(A || B)
   {
      if(A&1)
      {
         low++;
      }
      if(B&1)
      {
         if(low)
            low--;
      }
      
      A = A>>1;
      B = B>>1;
   }
   
   return low == 0;
}

u32 count_bits(u32 x)
{
   int bits;
   while(x)
   {
      if(x&1)
         bits++;
      x = x>>1;
   }
   return bits;
}

u32 high_bit(u32 x)
{
   u32 bit=0;
   while(x)
   {
      bit++;
      x = x>>1;
   }
   return bit-1;
}

u32 low_bit(u32 x)
{
   u32 bit=0;
   while(x)
   {
      bit++;
      if(x&1)
         return bit-1;
      x = x>>1;  
   }
   return bit-1;
}

int count(llist* list)
{
   int num = 0;
   lnode* node = list->head;
   while(node)
   {
      num++;
      node = node->next;
   }
   return num;
}

llist* create_list()
{
   llist* list = malloc(sizeof(llist));
   list->head = 0;
   return list;
}

lnode* create_node(void* data)
{
   lnode* nd = malloc(sizeof(lnode));
   nd->prev = 0;
   nd->next = 0;
   nd->data = data;
   return nd;
}

void push_front(llist* list, void* data)
{
   lnode* nd = create_node(data);
   
   if(list->head == 0)
   {
      list->head = nd;
   }
   else
   {
      nd->next = list->head;
      list->head->prev = nd->next;
      list->head = nd;
   }
}

void print_set(int* set, size_t num)
{
   int i = 0;
   while(i < num)
   {
      printf("%d ", set[i]);
      i++;
   }
   printf("= %d\n",set_sum(set,num));
}

int set_sum(int* set, size_t num)
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
