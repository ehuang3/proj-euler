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

#define LOW ((u64)-1>>32)

/****************************************************************************
  Implementation                       
****************************************************************************/
BI* BI_Create(u32 value, u32 sign)
{
	BI* b   = malloc(sizeof(BI));
	b->byte = malloc(sizeof(int));
	b->len  = 1;
	b->signum  = sign > 0 ? 1 : -1; // Safety...
	b->signum = value == 0 ? 0 : b->signum;
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

void BI_Free(BI* b)
{
	free(b->byte);
	free(b);
}

char* BI_ToString(BI* b)
{
	char* rev_str = malloc((b->len*10+1)*sizeof(char));
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
	char* string = malloc( (itr_rev-rev_str)*sizeof(char)+1 );
	char* itr_str = string;
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

int BI_isZero(BI* b)
{
	return b->len==1 && b->byte[b->offs]==0;
}

void BI_MultOneWord(BI* dst, BI* src, u32 val)
{
	// Zero case
	if(!val)
	{
		dst->byte[dst->offs] = 0;
		dst->len = 1;
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

void BI_Mult(BI* dst, BI* src, BI* mpr);
void BI_Sqr(BI* dst, BI* src);

void BI_AddOneWord(BI* dst, BI* src, u32 val)
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

void BI_Add(BI* dst, BI* src, BI* adr)
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
	// Catch edge cases
	if(val == 0)
	{
		BI_Copy(dst,src);
		return;
	}
	if(src->len == 1 && src->byte[src->offs] < val)
	{
		BI_Free(dst);
		dst = BI_Create(val-src->byte[src->offs],-1);
		return;
	}
	
	
	// src > val
	
}

void BI_Sub(BI* dst, BI* src, BI* sbr);

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


