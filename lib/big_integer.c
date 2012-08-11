#include "big_integer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************
  Preprocessor defines                       
****************************************************************************/
#define MIN_LEN (1);
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
	
	// Maybe no need to zero....
	size_t itr = nsize-psize;
	while(itr-- > 0)
	{
		b->bytes[psize + itr] = 0;
	}
}

#define LOW ((u64)-1>>32)
void mult_64x64(u64 a, u64 b, u64* buf)
{
	*buf++ = a*b;
	*buf  = (a&LOW)*(b&LOW)>>32;
	*buf += (a>>32)*(b&LOW);
	*buf += (a&LOW)*(b>>32);
	*buf  = *buf>>32;
	*buf += (a>>32)*(b>>32);
}

void add_2x64(u64 a, u64 b, u64* buf)
{
	*buf++ = a+b;
	*buf 	= (a>>1)+(b>>1);
	*buf += (a&0x1)+(b&0x1)>>1;
	*buf  = *buf>>63;
}

void add_3x64(u64 a, u64 b, u64 c, u64* buf)
{
	*buf++ = a+b+c;
	*buf 	= (a>>2)+(b>>2)+(c>>2);
	*buf += (a&0x3)+(b&0x3)+(c&0x3)>>2;
	*buf  = *buf>>62;
}

/****************************************************************************
  Implementation                       
****************************************************************************/
BI* BIcreate(long value)
{
	BI* b = malloc(sizeof(BI));
	b->bytes = malloc( sizeof(long) * 1 );
	b->b_len = MIN_LEN;
	b->s_len = MIN_CHAR_LEN;
	b->string = malloc(sizeof(char) * 20 );
	// Zero
	size_t itr = MIN_LEN;
	while(itr--)
	{
		b->bytes[itr] = 0;
	}
	b->sign = value >= 0 ? 1 : -1;
	b->bytes[0] = value*b->sign;
	
	return b;
}

void BIcopy(BI* dst, BI* src)
{
	if(dst->b_len != src->b_len)
	{
		// realloc, no zeroing
		dst->b_len = src->b_len;
		realloc(dst->bytes, sizeof(long)*dst->b_len);
	}
	
	dst->sign = src->sign;
	memcpy(dst->bytes, src->bytes, sizeof(long)*dst->b_len);
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

int BIcompare(const void* B1, const void* B2)
{
	BI* b1 = (BI*)B1;
	BI* b2 = (BI*)B2;
	
	if(b1->sign > b2->sign)
		return 1;
	if(b1->sign < b2->sign)
		return -1;
		
	return BIabs_compare(B1,B2)*b1->sign;
}

int BIabs_compare(const void* B1, const void* B2)
{
	BI* b1 = (BI*)B1;
	BI* b2 = (BI*)B2;
	
	// Assume compactness of representation
	if(b1->b_len > b2->b_len)
		return 1;
	if(b1->b_len < b2->b_len)
		return -1;
	
	// Same length, same sign
	u64* byte1 = b1->bytes;
	u64* byte2 = b2->bytes;
	
	u64 i = b1->b_len;
	while(i--)
	{
		if(byte1[i] < byte2[i])
			return -1;
		else if(byte1[i] > byte2[i])
			return 1;
	}
	
	return 0;
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

void BIsub(BI* dst, BI* op1, BI* op2)
{
	// Swap 
	int g = BI_compare(op1,op2);
	if(g < 0)
	{
		BI* tmp = op1;
		op1 = op2;
		op2 = tmp;
	}
	
	// Precondition: |op1| is greater than |op2|? Nope.
}

void BImult(BI* dst, BI* op1, BI* op2)
{
	// byteA is the shorter byte
	// byteB is the longer byte
	size_t lenA = op1->b_len;
	size_t lenB = op2->b_len;
	u64* byteA = op1->bytes;
	u64* byteB = op2->bytes;
	if(lenA > lenB)
	{
		lenA = op2->b_len;
		lenB = op1->b_len;
		byteA = op2->bytes;
		byteB = op1->bytes;
	}
	
	// Temporary storage
	// +1 to protect against the write of tmp[i+2] at i = n+m-2
	u64* tmp = malloc(sizeof(long)*(lenA+lenB)+1); 
	
	/* Let byteA be n u64's, byteB be m u64's.
	 * number of bytes of result is n+m, in indexes that's {0,...,n+m-1}
	 * byteA indexes {0,...,n-1}
	 * byteB indexes {0,...,m-1}
	 * each multiplication of u64 is 128 bits 
	 * so n+m-1 u64 is overflow of [n-1]*[m-1] multiplication, solely
	 */
	long max_i = lenA+lenB-2; // Iterate up to n+m-2
	tmp[0] = 0; tmp[1] = 0; // Prepare for loop
	/* To account for overflows properly, we calculate lbytes in sequence */
	for(long i=0; i <= max_i; i++)
	{
		
		tmp[i+2] = 0; // Prepare for inner loop
		/* These are lbytes of A and B that multiply into the current lbyte i */
		for(long j=0; j < lenA && i-j >= 0; j++)
		{
			u64 ia = j;
			u64 ib = i-j;
			
			u64 mbuf[2];
			// Buffer 128 bit result of multiplication
			mult_64x64(byteA[ia],byteB[ib],&mbuf[0]);
			
			u64 abuf[3];
			// Buffer result of addition of byteA*byteB to tmp
			add_2x64(mbuf[0],tmp[i],&abuf[0]);
			tmp[i] = abuf[0];
			
			add_3x64(mbuf[1],abuf[1],tmp[i+1],&abuf[1]);
			tmp[i+1] = abuf[1];
			// Assume that overflow of i+2 cell will not happen
			// It would take ~ 2^64 additions/u64's
			tmp[i+2] += abuf[2];
		}
	}
	
	// Copy
	
	// Assume that byteA and byteB are compact (every lbyte is nonzero)
	if( tmp[lenA+lenB-1] )
	{
		if(dst->b_len != lenA+lenB)
			BIrealloc(dst, lenA+lenB);
			
		memcpy(dst->bytes, tmp, sizeof(long)*(lenA+lenB));
	}
	else
	{
		// Last lbyte was empty
		if(dst->b_len != lenA+lenB-1)
			BIrealloc(dst, lenA+lenB-1);
			
		memcpy(dst->bytes, tmp, sizeof(long)*(lenA+lenB-1));
	}
	
	// Free
	free(tmp);
}

void BIdiv(BI* dst, BI* opd, BI* opr)
{
	//TODO: Implement!!! ugh.
}

void BImod(BI* dst, BI* opd, BI* opr)
{
	//TODO: Implement!
}

void BIshiftL(BI* dst, BI* src, int n)
{
	
}

void BIshiftR(BI* dst, BI* src, int n)
{
	
}













