#include <stdlib.h>
#include <stdio.h>

typedef unsigned int u32;

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

llist* create_list();
lnode* create_node(void* data);
void push_front(llist* list, void* data);
int count(llist* list);

int calc_pairs(llist* list, size_t num);
u32 count_bits(u32 x);
u32 high_bit(u32 x);
u32 low_bit(u32 x);
u32 bit_compare(u32 A, u32 B);

llist p7 = {0};

int main(void)
{  
   printf("%#x\n",-1);
   printf("%#x\n", high_bit(0x0));

   printf("12 = %d\n", calc_pairs(&p7, 12));
   
   u32 A = 0x55;
   u32 B = 0xAA;
   
   printf("%#x < %#x = %s\n", A, B, bit_compare(A,B) ? "true" : "false");
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
