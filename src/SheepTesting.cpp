#include <iostream>
#include <stdio.h>

//using namespace std;

double pow(double p, int n);
double P(int n, double p);
double expected(double p);

static double known[10001];
static double unknown[10001];
int max = 10001;
int main()
{
   std::cout <<       "\tProblem 352\n";
   std::cout << "===========================" << std::endl;

   double p;
   double sum = 0;
   for(p = 0.01; p <= 0.50; p+=0.01)
   {
      for(int i=0; i<max; i++) 
      {
         known[i] = 0;
         unknown[i] = 0;
      }

      unknown [1] = 1;
      for(int n=2; n<max; n++)
      {
         double min = max;
         double Pn = P(n,p);
         for(int k=1; k<=n-1; k++)
         {
            double Pk = P(k,p);
            double E = 1 + (Pk/Pn)*(known[k] + unknown[n-k]) + (1-Pk/Pn)*(known[n-k]);
            if(E < min)
               min = E;
         }
         known[n] = min;
         min = max;
         for(int k=1; k<=n; k++)
         {
            double Pk = P(k,p);
            double E = unknown[n-k] + 1 + Pk*known[k];
            if(E < min)
               min = E;
         }
         unknown[n] = min;
      }

      sum += unknown[10000];
      printf("%f = %.9f\n",p,unknown[10000]);
   }
   
   printf("ans = %.7f\n",sum);
}

double P(int n, double p)
{
   return 1-pow(1-p,n);
}

double pow(double p, int n)
{
   double ans = 1;
   double exp = p;
   while( n )
   {
      if( n & 1 )
         ans *= exp;
      exp *= exp;
      n >>= 1;
   }
   return ans;
}
