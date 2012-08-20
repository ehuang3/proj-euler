#include <stdio.h>
#include "big_integer.h"

int main(void)
{
	printf("BigInteger Testing:\n\n");
	
	printf("#1- Creating a BigInteger:\n");
	BI* b = BI_Create(1087491031,1);
	BI_PrintDeci(b);
	printf("\n");
	
	printf("#2- Trying different prints:\n");
	BI_PrintHex(b);
	BI_PrintBits(b);
	BI_PrintDeci(b);
	printf("\n");
	
	printf("#3- Mulitply it with a word:\n");
	BI_MultOneWord(b,b,3215728912);
	BI_PrintDeci(b);
	printf("\n");
	
	printf("#4- Multiply it with 0:\n");
	BI_MultOneWord(b,b,0);
	BI_PrintDeci(b);
	printf("\n");
	
	printf("#5- Adding it with a word:\n");
	BI_AddOneWord(b,b,1239112);
	BI_PrintDeci(b);
	printf("\n");

	printf("#6- Adding it with 0:\n");
	BI_AddOneWord(b,b,0);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	printf("\n");
	
	printf("#7- Adding it with 4,294,967,295:\n");
	BI_AddOneWord(b,b,4294967295);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	printf("\n");
	
	printf("#8- Set to 0 and add it 1024 times: \n");
	BI_MultOneWord(b,b,0);
	for(int i=0; i<1024; i++)
		BI_AddOneWord(b,b,4294967295);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	printf("\n");
	
	printf("#9- Double it by adding to itself: \n");
	BI_Add(b,b,b);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	printf("\n");
	
	printf("#10- Double it 85 more times and add words: \n");
	for(int i=0; i<84; i++){
		BI_Add(b,b,b);
		BI_AddOneWord(b,b,-1);
	}
	BI_PrintDeci(b);
	BI_PrintHex(b);
	printf("\n");
	
	printf("#11- Add lots of words:\n");
	for(int i=0; i<50121; i++)
	{
		BI_AddOneWord(b,b,i*12342);
	}
	BI_AddOneWord(b,b,1);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	printf("\n");
	
	printf("#12- Resting BI to 2:\n");
	BI_Free(b);
	b = BI_Create(2,1);
	BI_PrintDeci(b);
	printf("\n");
	
	printf("#13- Testing BI_USubOneWord:\n");
	BI_USubOneWord(b,b,1);
	BI_PrintDeci(b);
	printf("\n");
	
	printf("#14- Testing carry of subtracting a word: \n");
	for(int i=0; i<64; i++)
	{
		BI_Add(b,b,b);
	}
	printf("Before: \n"); BI_PrintHex(b);
	BI_USubOneWord(b,b,-1);
	printf("After: "); BI_PrintHex(b);
	BI_AddOneWord(b,b,-1);
	printf("Rest: "); BI_PrintHex(b);
	printf("\n");
	
	printf("#15- Testing BI_USub: \n");
	BI* a = BI_Create(-1,1);
	printf("A: "); BI_PrintHex(a);
	printf("B: "); BI_PrintHex(b);
	BI_USub(b,b,a);
	printf("Sub: "); BI_PrintHex(b);
	BI_Add(b,b,a);
	printf("Rest: "); BI_PrintHex(b);
	printf("\n");
	
	printf("#16- Testing BI_USub: \n");
	for(int i=0; i<16; i++){
		BI_Add(a,a,a);
	}
	printf("A: "); BI_PrintHex(a);
	printf("B: "); BI_PrintHex(b);
	BI_USub(b,b,a);
	printf("Sub: "); BI_PrintHex(b);
	BI_Add(b,b,a);
	printf("Rest: "); BI_PrintHex(b);
	printf("\n");
	
	printf("#END- Free BigInteger(s):\n");
	BI_Free(b);
	printf("Free'd\n\n");
}
