# include <iostream>

using namespace std;

struct A {
   int prop_sum;
   int color;
   bool cyclic;
};
void dfs();
#define MAX 1000000
A n[MAX+10];
int main() {
   cout <<       "\tProblem 095\n";
   cout << "===========================" << endl;
   int max_sum = 0;
   for(int i=1; i<=MAX; i++) {
      for(int m=2; m*i<=MAX; m++) {
         n[i*m].prop_sum += i;
      }
      //cout << n[i] << endl;
      if(n[i].prop_sum > max_sum)
         max_sum = n[i].prop_sum;
   }
   //cout << max_sum << endl;

   dfs();
   
   int best_min = MAX;
   int best_length = 0;
   for(int i=1; i<=MAX; i++) {
      if(n[i].cyclic) {
         int min = i;
         int length = 1;
         int curr = n[i].prop_sum;
         while(curr != i) {
            if(curr < min)
               min = curr;
            length++;
            curr = n[curr].prop_sum;
         }
         if(length > best_length) {
            best_min = min;
            best_length = length;
         }
      }
   }

   cout << best_min << endl;
}

void dfs_visit(int);

void dfs() {
   for(int i=1; i<=MAX; i++) {
      if(n[i].color == 0)
         dfs_visit(i);
   }
}

void dfs_visit(int i) {
   n[i].color++;

   int j = n[i].prop_sum;
   if(j > MAX || j == 0)
      j = 0;
   else if(n[j].color == 0)
      dfs_visit(j);
   else if(n[j].color == 1)
      n[j].cyclic = true;

   n[i].color++;
}
