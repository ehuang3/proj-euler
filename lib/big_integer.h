#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H


#include "types.h"
#include <string.h>



/*******************************************************************************
  Typedef
*******************************************************************************/

typedef struct BIG_INTEGER
{
	int signum; 	// Sign
	u32* byte; 		// Big Endian byte array
	size_t len;		// Working length of array
	size_t offs;	// Number of unused words
} BigInteger;

typedef BigInteger BI;

/*******************************************************************************
  Function
*******************************************************************************/
BI* BI_Create(u32 value, u32 sign);
void BI_Copy(BI* dst, BI* src);

void BI_Free(BI* b);

char* BI_ToString(BI* b);

void BI_PrintHex(BI* b);
void BI_PrintBits(BI* b);
void BI_PrintDeci(BI* b);

int BI_Compare(const void* A, const void* B);
int BI_ComparePtr(const void* A, const void* B);
int BI_CompareMag(const void* A, const void* B);

int BI_isZero(BI* b);
/* Sets input BigInteger to zero */
void BI_Zero(BI* dst);

/* Wrapper functions implement signed multiplication */
void BI_MultOneWord(BI* dst, BI* src, u32 val);
void BI_Mult(BI* dst, BI* src, BI* mpr);
/* Unsigned add */
void BI_UMultOneWord(BI* dst, BI* src, u32 val);
void BI_UMult(BI* dst, BI* src, BI* mpr);

void BI_Sqr(BI* dst, BI* src);

/* Wrapper functions implement signed addition */
void BI_AddOneWord(BI* dst, BI* src, u32 val);
void BI_Add(BI* dst, BI* src, BI* adr);
/* Unsigned add */
void BI_UAddOneWord(BI* dst, BI* src, u32 val);
void BI_UAdd(BI* dst, BI* src, BI* adr);

/* Wrapper functions implement signed subtraction */
void BI_SubOneWord(BI* dst, BI* src, u32 val);
void BI_Sub(BI* dst, BI* src, BI* sbr);
/* Unsigned subtract */
void BI_USubOneWord(BI* dst, BI* src, u32 val);
void BI_USub(BI* dst, BI* src, BI* sbr);

u32 BI_DivOneWord(BI* dst, BI* src, u32 val);
void BI_Div(BI* dst, BI* rm, BI* src, BI* dvr);

u32 BI_ModOneWord(BI* src, u32 val);
void BI_Mod(BI* dst, BI* src, BI* dvr);

#endif
