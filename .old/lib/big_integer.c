#include "big_integer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****************************************************************************
  Creating and Destroying
****************************************************************************/
BI* BI_Create(u32 value, u32 sign)
{
	DEBUG_ASSERT(sign >= -1 && sign <= 1);

	BI* b   = malloc(sizeof(BI));
	b->byte = malloc(sizeof(int));
	b->len  = 1;
	b->signum  = sign;
	b->byte[0] = value;
	b->offs = 0;
	return b;
}

void BI_Copy(BI* dst, BI* src)
{
	dst->len = src->len;
	dst->signum = src->signum;
	dst->offs = 0;
	free(dst->byte);
	dst->byte = malloc(src->len * sizeof(int));
	memcpy(dst->byte, &src->byte[src->offs], src->len * sizeof(int));
}

BI* BI_CopyCreate(BI* src)
{
	BI* dst = BI_Create(0,0);
	BI_Copy(dst,src);
	return dst;
}

BI* BI_StringCreate(char* string)
{
	BI* dst = BI_Create(0,0);
	BI* dig = BI_Create(1,1);
	BI* pow = BI_Create(1,1);

	int neg = 0;
	if(*string == '-')
	{
		neg = 1;
		string++;
	}

	char* itr_str = string;
	size_t len = 0;
	while(*itr_str != 0) // Go to end of string;
	{
		itr_str++;
		len ++;
	}
	itr_str--;

	if(len == 1 && *itr_str == '0')
	{
		//it's zero
		dst->signum = 0;
	}
	else
	{
		while(len--)
		{
			BI_MultOneWord(dig,pow, *itr_str-- - '1'+1);
			BI_UAdd(dst,dst,dig);
			BI_MultOneWord(pow,pow,10);
		}

		if(neg)
			dst->signum = -1;
		else
			dst->signum = 1;
	}

	BI_Free(dig);
	BI_Free(pow);

	return dst;
}

void BI_Free(BI* b)
{
	free(b->byte);
	free(b);
}

/*******************************************************************************
  Printing
*******************************************************************************/
char* BI_ToString(BI* b)
{
	char* rev_str = malloc((b->len*10+2)*sizeof(char));
	char* itr_rev = rev_str;
	BI* t = BI_Create(0,0); BI_Copy(t,b);
	if(BI_isZero(t))
	{
		sprintf(rev_str,"%d",0);
		rev_str[1] = 0;
		BI_Free(t);
		return rev_str;
	}
	while(!BI_isZero(t))
	{
		u32 digit = BI_DivOneWord(t,t,10);
		sprintf(itr_rev++,"%d",digit);
	}
	// Reverse
	char* string = malloc( (itr_rev-rev_str)*sizeof(char)+2 );
	char* itr_str = string;
	if(b->signum == -1)
		*itr_str++ = '-';
	u32 i = itr_rev-rev_str;
	while(i--)
	{
		*itr_str++ = *--itr_rev;
	}
	*itr_str = 0; // 0 terminator
	free(rev_str);
	BI_Free(t);
	return string;
}

void BI_PrintHex(BI* b)
{
	u32 i = b->len;
	u32* byte = &b->byte[b->offs];
	while(i--)
	{
		printf("%.8x ",*byte++);
	}
	printf("\n");
}

void BI_PrintBits(BI* b)
{
	u32 i = b->len;
	u32* byte = &b->byte[b->offs];
	while(i--)
	{
		u32 j = 32;
		u32 buf = *byte++;
		while(j--)
		{
			if(buf&0x80000000)
				printf("1");
			else
				printf("0");
			buf = buf<<1;
			
			if(j%4 == 0)
				printf(" ");
		}
	}
	printf("\n");
}

void BI_PrintDeci(BI* b)
{
	char* string = BI_ToString(b);
	printf("%s\n",string);
	free(string);
}

/*******************************************************************************
  Comparison
*******************************************************************************/
int BI_Compare(const void* A, const void* B)
{
	BI* a = (BI*)A;
	BI* b = (BI*)B;
	
	if(a->signum > b->signum)
		return 1;
	if(b->signum > a->signum)
		return -1;
	
	return BI_CompareMag(A,B)*a->signum;
}

int BI_ComparePtr(const void* A, const void* B)
{
	BI* a = *(BI**)A;
	BI* b = *(BI**)B;
	return BI_Compare(a,b);
}

int BI_CompareMag(const void* A, const void* B)
{
	BI* a = (BI*)A;
	BI* b = (BI*)B;
	
	if(a->len > b->len)
		return 1;
	else if(b->len > a->len)
		return -1;
		
	u32* byteA = &a->byte[a->offs];
	u32* byteB = &b->byte[b->offs];
	u32 i = a->len;
	while(i--)
	{
		if(*byteA > *byteB)
			return 1;
		if(*byteB > *byteA)
			return -1;
		byteA++;
		byteB++;
	}
	return 0;
}

/*******************************************************************************
  Zero, Sign, Util
******************************************************************************/
int BI_isZero(BI* b)
{
	return b->len==1 && b->byte[b->offs]==0;// && b->signum==0;
}

void BI_Zero(BI* b)
{
	b->len = 1;
	b->byte[b->offs] = 0;
	b->signum = 0;
}

void BI_Neg(BI* b)
{
	b->signum *= -1;
}

void BI_Set(BI* b, u32 value, int sign)
{
	b->offs = 0;
	b->len = 1;
	b->byte[0] = value;
	b->signum = sign;
}

/*******************************************************************************
  Addition and subtraction
*******************************************************************************/
void BI_AddOneWord(BI* dst, BI* src, u32 val)
{
	int op_sign = src->signum;
	if(op_sign == 1)
	{
		// Bytes are to be added
		BI_UAddOneWord(dst,src,val);
		dst->signum = src->signum;
	}
	else if(op_sign == 0)
	{
		// src is 0
		BI_Set(dst, val, 1);
	}
	else if(op_sign == -1)
	{
		// Bytes are to be subtracted
		if(src->len == 1)
		{
			// BI is word length
			u32 byte = src->byte[src->offs];
			dst->len = 1;
			
			if(byte == val)
			{
				BI_Zero(dst);
			}
			else if(byte > val)
			{
				dst->byte[0] = byte-val;
				dst->signum = -1;
			}
			else if(byte < val)
			{
				dst->byte[0] = val-byte;
				dst->signum = 1;
			}	
		}
		else
		{
			// BI is not word length
			BI_USubOneWord(dst,src,val);
			dst->signum = -1;
		}
	}
}

void BI_Add(BI* dst, BI* src, BI* op)
{
	int op_sign = src->signum * op->signum;
	if(op_sign == 1)
	{
		// Magnitudes are to be added
		BI_UAdd(dst,src,op);
		dst->signum = src->signum;
	}
	else if(op_sign == 0)
	{
		// At least one BI is zero
		if(src->signum == 0)
		{
			BI_Copy(dst,op);
			dst->signum = op->signum;
		}
		else
		{
			BI_Copy(dst,src);
			dst->signum = src->signum;
		}
	}
	else if(op_sign == -1)
	{
		// Magnitudes are to be subtracted
		int cmp = BI_CompareMag(src,op);
		if(cmp == 1)
		{
			BI_USub(dst,src,op);
			dst->signum = src->signum;
		}
		else if(cmp == 0)
		{
			BI_Zero(dst);
		}
		else if(cmp == -1)
		{
			BI_USub(dst,op,src);
			dst->signum = src->signum*-1;
		}
	}
}

void BI_UAddOneWord(BI* dst, BI* src, u32 val)
{
	u32* tmp = malloc((src->len+1)*sizeof(int));
	u32* itr_tmp = &tmp[src->len];
	u32* byte = &src->byte[src->len-1+src->offs];
	
	u64 buffer = (u64)*byte-- + (u64)val;
	*itr_tmp-- = (u32)buffer;
	u32 i = src->len-1;
	while( i-- )
	{
		buffer=buffer>>32;
		buffer += (u64)*byte--;
		*itr_tmp-- = (u32)buffer;
	}
	*itr_tmp = buffer>>32;
	
	free(dst->byte);
	dst->byte = tmp;
	dst->offs = !*tmp ? 1 : 0;
	dst->len  = src->len+1 - dst->offs;
}

void BI_UAdd(BI* dst, BI* src, BI* adr)
{
	BI* bigL = src;
	BI* bigS = adr;
	if(src->len < adr->len)
	{
		bigL = adr;
		bigS = src;
	}
	u32* byteL = &bigL->byte[bigL->offs+bigL->len-1];
	u32* byteS = &bigS->byte[bigS->offs+bigS->len-1];
	size_t max_len = bigL->len;
	size_t min_len = bigS->len;

	// Intermediate storage for calculation
	u32* tmp = malloc(sizeof(int)*(max_len+1));
	u32* itr_tmp = &tmp[max_len];
	
	// Addition
	size_t i = min_len;
	*itr_tmp = 0;
	while(i--)
	{
		u64 buffer = (u64)*itr_tmp + (u64)*byteL + (u64)*byteS;
		*itr_tmp   = (u32)buffer;
		*(itr_tmp-1) = buffer>>32;
		
		itr_tmp--;
		byteL--;
		byteS--;
	}
	
	// Add unaligned bytes
	i = max_len - min_len;
	while(i--)
	{
		u64 buffer = (u64)*byteL + (u64)*itr_tmp;
		*itr_tmp   = (u32)buffer;
		*(itr_tmp-1) = buffer>>32;
		
		itr_tmp--;
		byteL--;
	}
	
	free(dst->byte);
	dst->byte = tmp;
	dst->offs = !*tmp ? 1 : 0;
	dst->len  = max_len+1-dst->offs;
}

void BI_SubOneWord(BI* dst, BI* src, u32 val)
{
	int op_sign = src->signum;
	if(op_sign == 1)
	{
		// Magnitudes are to be subtracted
		if(src->len == 1)
		{
			// BI is word length
			u32 byte = src->byte[src->offs];
			dst->len = 1;
			
			if(byte == val)
			{
				BI_Zero(dst);
			}
			else if(byte > val)
			{
				dst->byte[dst->offs] = byte-val;
				dst->signum = 1;
			}
			else if(byte < val)
			{
				dst->byte[dst->offs] = val-byte;
				dst->signum = -1;
			}
		}
		else
		{
			// BI is not word length
			BI_USubOneWord(dst,src,val);
		}
	}
	else if(op_sign == 0)
	{
		// src is 0
		BI_Set(dst,val,-1);
	}
	else if(op_sign == -1)
	{
		// Magnitudes are to be added
		BI_UAddOneWord(dst,src,val);
		dst->signum = src->signum;
	}
}

void BI_Sub(BI* dst, BI* src, BI* op)
{
	int op_sign = src->signum * op->signum;
	if(op_sign == 1)
	{
		// Magnitudes are to be subtracted
		int cmp = BI_CompareMag(src,op);
		if(cmp == 1)
		{
			BI_USub(dst,src,op);
			dst->signum = src->signum;
		}
		else if(cmp == 0)
		{
			BI_Zero(dst);
		}
		else if(cmp == -1)
		{
			BI_USub(dst,op,src);
			dst->signum = src->signum*-1;
		}
	}
	else if(op_sign == 0)
	{
		// At least one BI is zero
		if(src->signum == 0)
		{
			BI_Copy(dst,op);
			dst->signum = op->signum*-1;
		}
		else
		{
			BI_Copy(dst,src);
			dst->signum = src->signum;
		}
	}
	else if(op_sign == -1)
	{
		// Magnitudes are to be added
		BI_UAdd(dst,src,op);
		dst->signum = src->signum;
	}
}

/* Precondition: |src| > |val| */
/* Postcondition: |dst| = |src|-|val| */
void BI_USubOneWord(BI* dst, BI* src, u32 val)
{
	u32* tmp = malloc(src->len*sizeof(int));
	u32* itr_tmp = &tmp[src->len-1];
	u32* byte = &src->byte[src->offs+src->len-1];
	
	u64 buffer = (u64)*byte-- - (u64)val;
	*itr_tmp-- = (u32)buffer;
	u32 i = src->len-1;
	while(i--)
	{
		buffer = (u64)*byte-- - ((buffer>>32)&1);
		*itr_tmp-- = (u32)buffer;
	}
	
	free(dst->byte);
	dst->byte = tmp;
	dst->offs = !*tmp ? 1 : 0;
	dst->len  = src->len-dst->offs;
}

/* Precondition: |src| > |op| */
void BI_USub(BI* dst, BI* src, BI* op)
{
	size_t max_len = src->len;
	size_t min_len = op->len;
	u32* byteS = &src->byte[src->offs+src->len-1];
	u32* byteO = &op->byte[op->offs+op->len-1];
	
	u32* tmp = malloc(max_len*sizeof(int));
	u32* itr_tmp = &tmp[max_len-1];
	
	// Subtract aligned bytes
	u32 i = min_len;
	u64 buffer = 0;
	while(i--)
	{
		buffer = (u64)*byteS-- - (u64)*byteO-- - ((buffer>>32)&1);
		*itr_tmp-- = (u32)buffer;
	}
	// Carry-through unaligned bytes
	i = max_len-min_len;
	while(i--)
	{
		buffer = (u64)*byteS-- - ((buffer>>32)&1);
		*itr_tmp-- = (u32)buffer;
	}
	
	// Count zero bytes
	itr_tmp = tmp;
	while(!*itr_tmp)
	{
		itr_tmp++;
	}
	
	free(dst->byte);
	dst->byte = tmp;
	dst->offs = itr_tmp-tmp;
	dst->len  = max_len-dst->offs;
}

/*******************************************************************************
  Multiplication and Division
*******************************************************************************/
void BI_MultOneWord(BI* dst, BI* src, u32 val)
{
	// Zero case
	if(!val)
	{
		BI_Zero(dst);
		return;
	}
	
	u32* tmp = malloc((src->len+1)*sizeof(int));
	u32 i = src->len;
	u32* byte = &src->byte[src->offs];
	tmp[i] = 0;
	while(i--)
	{
		u64 buf = (u64)byte[i]*(u64)val + tmp[i+1];
		tmp[i] = buf>>32; // avoid the little endianess... 
		tmp[i+1] = (u32)buf;
	}
	free(dst->byte);
	dst->byte = tmp;
	dst->offs = !*tmp ? 1 : 0;
	dst->len  = src->len+1 - dst->offs;
}

void BI_Mult(BI* dst, BI* src, BI* mpr)
{

}

void BI_UMult(BI* dst, BI* src, BI* op)
{
	// byteA is the shorter byte array
	// byteB is the longer byte array
	BI* op1 = src, *op2 = op;
	if(op1->len < op2->len)
	{
		op1 = op;
		op2 = src;
	}
	size_t lenA = op1->len;
	size_t lenB = op2->len;
	u32* byteA = op1->byte;
	u32* byteB = op2->byte;

	// Temporary storage
	// +1 to protect against the write of tmp[i+2] at i = n+m-2 FIXME
	u32* tmp = malloc(sizeof(int)*(lenA+lenB+1));


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


void BI_Sqr(BI* dst, BI* src);

u32 BI_DivOneWord(BI* dst, BI* src, u32 val)
{
	u32* tmp = malloc(src->len*sizeof(int));
	u32* itr_tmp = tmp;
	u32 i = src->len;
	u32* byte = &src->byte[src->offs];
	u64 rem = 0;
	while(i--)
	{
		rem = (rem<<32) + *byte++;
		*itr_tmp++ = rem/val;
		rem = rem%val;
	}
	free(dst->byte);
	dst->byte = tmp;
	dst->offs = !*tmp && src->len>1 ? 1 : 0;
	dst->len  = src->len - dst->offs;
	return rem;
}

void BI_Div(BI* quo, BI* rem, BI* src, BI* dvr);

u32 BI_ModOneWord(BI* src, u32 val)
{
	u32 i = src->len;
	u32* byte = &src->byte[src->offs];
	u64 rem = 0;
	while(i--)
	{
		rem = (rem<<32) + *byte++;
		rem = rem%val;
	}
	return rem;
}

void BI_Mod(BI* dst, BI* src, BI* dvr);


