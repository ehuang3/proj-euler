#include <stdio.h>




int next(int* a, int* b, int* c)
{
	(*c) ++;
	if(*c < 10)
		return 1;
	else
		*c = 0;
	
	(*b) ++;
	if(*b < 10)
		return 1;
	else
		*b = 0;
	
	(*a) ++;
	if(*a < 10)
		return 1;
	
	*a = 0;
	*b = 0;
	*c = -1;
	
	return 0;
}

int fill_bottom(int matrix[][4], int col, int sum)
{
	matrix[3][col] = sum - matrix[0][col] - matrix[1][col] - matrix[2][col];
	
	if(matrix[3][col] < 10 && matrix[3][col] >= 0)
		return 1;
	return 0;
}

int diag_right(int m[][4], int sum)
{
	return m[0][0]+m[1][1]+m[2][2]+m[3][3] == sum;
}

int diag_left(int m[][4], int sum)
{
	return m[0][3]+m[1][2]+m[2][1]+m[3][0] == sum;
}

int main(void)
{
	int matrix[4][4] = {0};

	int a=0,b=0,c=-1;
	int e=0,f=0,g=-1;
	int i=0,j=0,k=-1;
	
	long count = 0;
	int msum = 0;
	while(msum < 37)
	{
		printf("msum = %d\n", msum);
		while(next(&a,&b,&c))
		{
			matrix[0][0] = a; matrix[0][1] = b; matrix[0][2] = c;
			matrix[0][3] = msum -a-b-c;
		
			if(matrix[0][3] < 10 && matrix[0][3] >= 0){
			while(next(&e,&f,&g))
			{
				matrix[1][0] = e; matrix[1][1] = f; matrix[1][2] = g;
				matrix[1][3] = msum -e-f-g;
			
				if(matrix[1][3] < 10 && matrix[1][3] >= 0){
				while(next(&i,&j,&k))
				{	
					matrix[2][0] = i; matrix[2][1] = j; matrix[2][2] = k;
					matrix[2][3] = msum -i-j-k;
					if(matrix[2][3] < 10 && matrix[2][3] >= 0)
					{
						if(fill_bottom(matrix, 0, msum))
						if(fill_bottom(matrix, 1, msum))
						if(fill_bottom(matrix, 2, msum))
						if(fill_bottom(matrix, 3, msum))
						if(diag_right(matrix, msum))
						if(diag_left(matrix, msum))
						{
							count ++;
						}
					}
				}}
			}}
		}
		
		msum++;
	}
	
	printf("count = %ld\n", count);
}
