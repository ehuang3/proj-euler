#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H


#include "types.h"
#include <string.h>



/*******************************************************************************
  Typedef                       
*******************************************************************************/
// TODO: Add valid bytes field, remove compactness requirment
typedef struct BIG_INTEGER
{
	int sign;
	u64* bytes;
	size_t b_len;
	char* string;
	size_t s_len;
} BigInteger;

typedef BigInteger BI;

/*******************************************************************************
  Function                       
*******************************************************************************/
BI* BIcreate(long value);
void BIcopy(BI* dst, BI* src);

void BIfree(BI* b);

char* BIstring(BI* b);
void BIprint(BI* b);

int BIcompare(const void* B1, const void* B2);
int BIabs_compare(const void* B1, const void* B2);

void BIadd(BI* dst, BI* op1, BI* op2);
void BIsub(BI* dst, BI* op1, BI* op2);
void BImult(BI* dst, BI* op1, BI* op2);
void BIdiv(BI* dst, BI* dvnd, BI* dvr);
void BImod(BI* dst, BI* dvnd, BI* dvr);

void BIshiftL(BI* dst, BI* src, int n);
void BIshiftR(BI* dst, BI* src, int n);

#endif
