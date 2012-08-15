#include <stdio.h>
#include "big_integer.h"

int main(void)
{
	printf("BigInteger Testing:\n\n");
	
	printf("#1- Creating a BigInteger:\n");
	BI* b = BI_Create(1087491031,1);
	BI_PrintDeci(b);
	
	printf("#2- Trying different prints:\n");
	BI_PrintHex(b);
	BI_PrintBits(b);
	BI_PrintDeci(b);
	
	printf("#3- Mulitply it with a word:\n");
	BI_MultOneWord(b,b,3215728912);
	BI_PrintDeci(b);
	
	printf("#4- Multiply it with 0:\n");
	BI_MultOneWord(b,b,0);
	BI_PrintDeci(b);
	
	printf("#5- Adding it with a word:\n");
	BI_AddOneWord(b,b,1239112);
	BI_PrintDeci(b);

	printf("#6- Adding it with 0:\n");
	BI_AddOneWord(b,b,0);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	
	printf("#7- Adding it with 4,294,967,295:\n");
	BI_AddOneWord(b,b,4294967295);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	
	printf("#8- Set to 0 and add it 1024 times: \n");
	BI_MultOneWord(b,b,0);
	for(int i=0; i<1024; i++)
		BI_AddOneWord(b,b,4294967295);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	
	printf("#9- Double it by adding to itself: \n");
	BI_Add(b,b,b);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	
	printf("#10- Double it 85 more times and add words: \n");
	for(int i=0; i<84; i++){
		BI_Add(b,b,b);
		BI_AddOneWord(b,b,-1);
	}
	BI_PrintDeci(b);
	BI_PrintHex(b);
	
	printf("#11- Add lots of words:\n");
	for(int i=0; i<50121; i++)
	{
		BI_AddOneWord(b,b,i*12342);
	}
	BI_AddOneWord(b,b,1);
	BI_PrintDeci(b);
	BI_PrintHex(b);
	
	printf("#END- Free BigInteger(s):\n");
	BI_Free(b);
	printf("Free'd\n\n");
}
