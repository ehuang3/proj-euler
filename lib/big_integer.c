#include "big_integer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************
  Preprocessor defines                       
****************************************************************************/
#define MIN_LEN (4);
#define MIN_CHAR_LEN (20);
#define SIG_BIT (1 << SIG_BIT_IND)
#define SIG_BIT_IND (sizeof(long)*8-1)


/****************************************************************************
  
typdef struct BIG_INTEGER
{
	long* bytes;
	size_t b_len;
	char* string;
	ssize_t s_len;
} BigInteger; 

****************************************************************************/

/****************************************************************************
  Internal methods                       
****************************************************************************/
void BIrealloc(BI* b, size_t nsize)
{
	size_t psize = b->b_len;
	
	b->bytes = realloc(b->bytes, sizeof(long)*nsize);
	b->b_len = nsize;
	
	size_t itr = nsize-psize;
	while(itr-- > 0)
	{
		b->bytes[psize + itr] = 0;
	}
}

/****************************************************************************
  Implementation                       
****************************************************************************/
BI* BIcreate(long value)
{
	BI* b = malloc(sizeof(BI));
	b->bytes = malloc( sizeof(long) * 4 );
	b->b_len = MIN_LEN;
	b->s_len = MIN_CHAR_LEN;
	b->string = malloc(sizeof(char) * 20 );
	// Zero
	size_t itr = MIN_LEN;
	while(itr--)
	{
		b->bytes[itr] = 0;
	}
	b->bytes[0] = value;
	
	return b;
}

void BIfree(BI* b)
{
	free(b->string);
	free(b->bytes);
	free(b);
}

char* BIstring(BI* b)
{
	return NULL;
}

void BIprint(BI* b)
{
	size_t itr = 0;
	u32* byte = (u32*)b->bytes;
	while(itr < b->b_len)
	{
		printf("%ld:x%.8x:%.8x ", 
					itr*sizeof(long), *byte++, *byte++);
		itr++;
		if(itr%4 == 0)
			printf("\n");
	}
	if(itr%4 != 0)
		printf("\n");
}

void BIadd(BI* dst, BI* op1, BI* op2)
{
	u64* byteL = op1->bytes; // The longer byte array
	u64* byteS = op2->bytes; // The shorter byte array
	size_t max_len = op1->b_len;
	size_t min_len = op2->b_len;
	if(max_len < min_len)
	{
		max_len = op2->b_len;
		min_len = op1->b_len;
		byteL = op2->bytes;
		byteS = op1->bytes;
	}
	// Intermediate storage for calculation
	u64* imval = malloc(sizeof(long)*max_len + 1);
	u64* itmv = imval;
	
	// Addition
	size_t itr = min_len-max_len > 0 ? min_len : min_len-1;
	*itmv = 0;
	while(itr--)
	{
		// Handle byte overflow of *itmv + *btyeL + *byteS
		// by mimicking the addition of a 65 bit number
		u64 buf = (*byteL>>1) + (*byteS>>1);
		buf += (*itmv&1) + (*byteL&1) + (*byteS&1)>>1;
		*(itmv+1) = buf>>SIG_BIT_IND;
		
		*itmv += *byteL + *byteS;
		
		itmv++;
		byteL++;
		byteS++;
	}
	// Add unaligned bytes
	itr = max_len-min_len;
	if(itr--) // do last addition later
		while(itr--)
		{
			u64 buf = *byteL>>1;
			buf += (*itmv&1) + (*byteL&1)>>1;
			*(itmv+1) = buf>>SIG_BIT_IND;
			
			*itmv += *byteL;
			
			itmv++;
			byteL++;
		}
	// Check mem overflow in last addition
	u64 lb = min_len == max_len ? *byteS : 0;
	u64 fbuf = (*itmv>>1) + (*byteL>>1) + (lb>>1);
	fbuf += (*itmv&1) + (*byteL&1) + (lb&1)>>1;
	
	*(itmv+1) = fbuf>>SIG_BIT_IND;
	
	*itmv += *byteL + lb;
	itmv++;
	
	size_t nele = max_len;
	if(*itmv) // Mem overflow
	{
		BIrealloc(dst, dst->b_len + 1);
		nele ++;
	}
	
	// Copy to dst
	memcpy(dst->bytes, imval, sizeof(long)*nele);
	
	// Free
	free(imval);
}

void BImult(BI* dst, BI* op1, BI* op2)
{
	size_t lenA = op1->b_len;
	size_t lenB = op2->b_len;
	size_t max_len = lenA > lenB ? lenA : lenB;
	
	// Temporary storage
	u64* tmp = malloc(sizeof(long) * max_len * max_len);
	// Zeroing required
	size_t t = max_len*max_len;
	while(t--)
	{
		tmp[t] = 0;
	}
	
	u64* byteA = op1->bytes, *byteB;
	long ia = 0, ib = 0;
	
	long i, max = max_len*max_len;
	for(i=0; i < max; i++)
	{
		
	
	
	
	
	}
	
	
	
	
	
	
	
	while( ia < lenA )
	{
		byteB = op2->bytes;
		ib = lenB;
		while( ib < lenB )
		{
			// Buffer [64.127] of the op a[0.63]*b[0.63] = [0.127] ie [0.63][64.127]
			u64 buf = (*byteA>>31)*(*byteB)>>31;
			buf += (*byteA)*(*byteB>>31)>>31;
			buf += (*byteA>>31)*(*byteB>>31);
			
			// Check overflow of tmp[] += [0.63]
			u64* byte = &tmp[ia*ib];
			u64 low = *byteA * *byteB;
			
			u64 oflo = (low>>1) + (*byte>>1);
			oflo += (low &1) + (*byte &1);
			
			u64 ofhi = 0;
			
			tmp[ia*ib +1] += oflo>>63;
			
			tmp[ia*ib] += *byteA * *byteB;
			
			// Check overflow of tmp[] += [64.127]
			
			
			byteB++;
			ib++;
		}
		byteA++;
		ia++;
	}
}

void BIdiv(BI* dst, BI* ope, BI* opr)
{

}

void BImod(BI* dst, BI* ope, BI* opr)
{

}

void BIshiftL(BI* dst, BI* src, int n)
{
	
}

void BIshiftR(BI* dst, BI* src, int n)
{
	
}













