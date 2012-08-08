#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H


#include "types.h"
#include <string.h>



/*******************************************************************************
  Typedef                       
*******************************************************************************/
typedef struct BIG_INTEGER
{
	u8 sign;
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

void BIfree(BI* b);

char* BIstring(BI* b);
void BIprint(BI* b);

void BIadd(BI* dst, BI* op1, BI* op2);
void BImult(BI* dst, BI* op1, BI* op2);
void BIdiv(BI* dst, BI* ope, BI* opr);
void BImod(BI* dst, BI* ope, BI* opr);

void BIshiftL(BI* dst, BI* src, int n);
void BIshiftR(BI* dst, BI* src, int n);

#endif
