#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include "types.h"
#include <string.h>
#include <stdlib.h>

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
BI* BI_CopyCreate(BI* src);
BI* BI_StringCreate(char* string);

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

void BI_Neg(BI* b);

void BI_Set(BI* b, u32 value, int sign);

/* Wrapper functions implement signed multiplication */
void BI_MultOneWord(BI* dst, BI* src, u32 val);
void BI_Mult(BI* dst, BI* src, BI* mpr);
/* Unsigned mult */
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

/* Wrapper functions implement signed division */
u32 BI_DivOneWord(BI* dst, BI* src, u32 val);
void BI_Div(BI* dst, BI* rm, BI* src, BI* dvr);
/* Unsigned division */
u32 BI_UDivOneWord(BI* dst, BI* src, u32 val);
void BI_UDiv(BI* dst, BI* rm, BI* src, BI* dvr);

/* Modulus */
u32 BI_ModOneWord(BI* src, u32 val);
void BI_Mod(BI* dst, BI* src, BI* dvr);

/* Debugging Support */
#ifdef BI_DEBUG
    /*
       Does the following if compiled in debug mode
       When compiled in release mode does absolutely nothing.
    */
    #define IF_DEBUG(call) (call)
    /* Prints text (in red) if in debug mode */
    #define DEBUG_PRINT(string) fprintf(stderr, "\033[31m%s:%d %s\n\033[0m", __FILE__, __LINE__, (string))
    /* Asserts if the expression given is true (!0) */
    /* If this fails it prints a message and terminates */
    #define DEBUG_ASSERT(expr)   \
    do                           \
    {                            \
        if (!(expr))             \
        {                        \
            fprintf(stderr, "ASSERTION FAILED %s != TRUE (%d) IN %s ON line %d\n", #expr, (expr), __FILE__, __LINE__); \
            exit(0);             \
        }                        \
    } while(0)
#else
    #define IF_DEBUG(call)
    #define DEBUG_PRINT(string)
    #define DEBUG_ASSERT(expr)
#endif

#endif
