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
	
	BI* c = BI_Create(-1,-1);
	printf("#17- Creating negative numbers: \n");
	BI_PrintDeci(c);
	printf("\n");

	printf("#18- Testing BI_AddOneWord:\n");
	BI_AddOneWord(c,c,-1);
	BI_PrintDeci(c);
	printf("\n");

	BI_Set(c,-1,-1);
	printf("#19- Testing BI_Add of negative numbers:\n");
	BI_Add(c,c,c);
	BI_PrintDeci(c);
	printf("\n");

	BI_Set(b,9183451,1);
	BI_Add(c,c,b);
	BI_PrintDeci(c);
	printf("\n");

	printf("#20- Testing creation from string:\n");
	BI_Free(c);
	c = BI_StringCreate("-12345678901234567890");
	BI_PrintDeci(c);
	BI_Free(c);
	c = BI_StringCreate("0");
	BI_PrintDeci(c);
	BI_Free(c);
	c = BI_StringCreate("12345678900987654321");
	BI_PrintDeci(c);
	BI_Free(b);
	b = BI_StringCreate("-123123123123123");
	BI_PrintDeci(b);
	BI_Copy(a,c);
	BI_Neg(a);
	BI_PrintDeci(a);
	BI* d = BI_Create(0,0);
	BI_PrintDeci(d);
	printf("\n");

	printf("#21- Testing Add: \n");
	BI* out = BI_StringCreate("0");
	// b = -123123123123123
	// c =  12345678900987654321
	// a = -12345678900987654321

	// mag are added
	BI_Add(out,c,c);
	BI_PrintDeci(out);

	// mag are sub src > op
	BI_Add(out,c,b);
	BI_PrintDeci(out);

	// mag are sub src < op
	BI_Add(out,b,c);
	BI_PrintDeci(out);

	// mag are sub eq src == op
	BI_Add(out,a,c);
	BI_PrintDeci(out);

	BI_Add(out,a,d);
	BI_PrintDeci(out);
	BI_Add(out,d,a);
	BI_PrintDeci(out);

	printf("\n");

	printf("#22- Testing Sub:\n");
	// b = -123123123123123
	// c =  12345678900987654321
	// a = -12345678900987654321

	// mag are added
	BI_Sub(out,c,a);
	BI_PrintDeci(out);

	// mag are sub, src > op
	BI_Sub(out,a,b);
	BI_PrintDeci(out);

	// mag are sub, op > src
	BI_Sub(out,b,a);
	BI_PrintDeci(out);

	// mag are eq
	BI_Sub(out,a,a);
	BI_PrintDeci(out);

	BI_Sub(out,a,d);
	BI_PrintDeci(out);
	BI_Sub(out,d,a);
	BI_PrintDeci(out);

	printf("\n");

	BI_Free(a); BI_Free(b); BI_Free(c);

	printf("StringCreate is funny\n");
	a = BI_StringCreate("2oijfw1029oi");
	b = BI_StringCreate("-Alji319oifw");
	c = BI_StringCreate("-100000");
	BI_PrintDeci(a);
	BI_PrintDeci(b);
	BI_PrintDeci(c);
	printf("\n");

	printf("#23- Testing AddOneWord:\n");
	// a = 893411103587
	// b = -236373259311
	// c = 100000

	BI_AddOneWord(out,a,10);
	BI_PrintDeci(out);

	BI_AddOneWord(out,b,10);
	BI_PrintDeci(out);

	BI_AddOneWord(out,c,110000);
	BI_PrintDeci(out);

	BI_AddOneWord(out,d,10);
	BI_PrintDeci(out);

	printf("\n");

	BI_Set(c,1000,1);
	printf("#24- Testing SubOneWord: \n");
	// a = 893411103587
	// b = -236373259311
	// c = 100000
	BI_SubOneWord(out,a,10);
	BI_PrintDeci(out);

	BI_SubOneWord(out,b,10);
	BI_PrintDeci(out);

	BI_SubOneWord(out,c,99);
	BI_PrintDeci(out);

	BI_SubOneWord(out,d,10);
	BI_PrintDeci(out);

	printf("\n");



	BI_Free(out);


	printf("#END- Free BigInteger(s):\n");
	BI_Free(b); BI_Free(a); BI_Free(c); BI_Free(d);
	printf("Free'd\n\n");
}
