#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
  Typedefs                       
*******************************************************************************/
typedef struct Point
{
   int x;
   int y;
} Point;

typedef Point Line[2];

typedef Point Triangle[3];

typedef Point* Polygon;

/*******************************************************************************
  Prototypes                       
*******************************************************************************/
void parse_tri(Triangle* tri, char* line);
void print_tri(Triangle* tri);
int contains_pt(Polygon poly, size_t num, Point* pt);
int calc_part(Line line, Point* pt);
int interior_pt(Line line, Point pt); //Check if between endpoints, assume point is on line
int dot(Point* v, Point* w);
int magsq(Point* d);
Point delta(Point* v, Point* w);

/*******************************************************************************
  Main                       
*******************************************************************************/
int main(void)
{
   Triangle* tri = malloc(sizeof(Triangle));
   Point orig = {0,0};
   int count = 0;
   
   FILE* fp;
   char* line = NULL;
   size_t len = 0;
   ssize_t read;
   
   fp = fopen("./triangles.txt","r");
   
   while( (read = getline(&line, &len, stdin)) != -1)
   {
      printf("%s",line);
      parse_tri(tri, line);
      print_tri(tri);
      if(contains_pt((Point*)tri, 3, &orig))
      {
         printf("Contains origin\n");
         count++;
      }
      printf("\n");
   }
   
   printf("ans = %d\n",count);
}

/*******************************************************************************
  Implementation                       
*******************************************************************************/
/* Between endpoint check is actually not necessary */
int interior_pt(Line l, Point pt)
{
   printf("INTERIOR_PT\n");
   Point v = delta(&l[0],&l[1]);
   pt = delta(&l[0],&pt);
   int dd = dot(&v, &pt);
   if(dd < 0)
      return 0;
   if(dd*dd <= magsq(&v))
      return 1;
   return 0;
}

int dot(Point* v, Point* w)
{
   return v->x*w->x + v->y*w->y;
}

int magsq(Point* d)
{
   return d->x*d->x + d->y*d->y;
}

Point delta(Point* v, Point* w)
{
   Point dd = {w->x-v->x,w->y-v->y};
   return dd;
}

int calc_part(Line l, Point* pt)
{
   printf("CALC_PART:\n");
   int dx = l[1].x-l[0].x;
   int dy = l[1].y-l[0].y;
   Point* orig = &l[0];
   if(dx < 0)
   {
      dy *= -1;
      dx *= -1;
      orig = &l[1];
   }
   
   int dxyy0 = dx*(pt->y - orig->y);
   int dyxx0 = dy*(pt->x - orig->x);
   
   if(dxyy0 == dyxx0)
      return 0;
   if(dxyy0 > dyxx0)
      return 1;
   if(dxyy0 < dyxx0)
      return -1;
}

int contains_pt(Polygon poly, size_t num, Point* pt)
{
   printf("CONTAINS_PT:\n");
   int i = 0;
   while(i < num)
   {
      Line l;
      l[0] = poly[i];
      l[1] = poly[(i+1)%num];
      
      Point ref = poly[(i+2)%num];
      
      int ref_part = calc_part(l, &ref);
      int pt_part = calc_part(l, pt);
      
      //if(pt_part == 0 && interior_pt(l,*pt))
      //   return 1;
      if(ref_part != pt_part && pt_part != 0)
         return 0;
      
      i++;
   }
   return 1;
}

void parse_tri(Triangle* tri, char* line)
{
   //Domagiks, be n cahootz with te magiks
   int* t = (int*) tri;
   sscanf(line, "%d,%d,%d,%d,%d,%d",t++,t++,t++,t++,t++,t++); 
}

void print_tri(Triangle* tri)
{
   //I belif in magiks
   int* t = (int*)tri;
   printf("A(%d,%d),B(%d,%d),C(%d,%d)\n",*t++,*t++,*t++,*t++,*t++,*t++);
}
