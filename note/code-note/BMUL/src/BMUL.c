/*
 ============================================================================
 Name        : BMUL.c
 Author      : root
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "BMUL.h"

int main(void)
{
	/* Data init */
	char 	cArrNum_1[DATA_SIZE] 	= {0};
	char 	cArrNum_2[DATA_SIZE] 	= {0};
	char 	cArrRes[BUF_SIZE] 		= {0};
	int	iRet							= 0;

	/* input Num */
	printf("Input two Nums ( <= %dbit )\n", DATA_SIZE-1);
	printf("Num 1 = ");
#if DEBUG
	strcpy(cArrNum_1, "9876543219876");
	puts("" );
#else
	scanf("%s", cArrNum_1);
	if(cArrNum_1[DATA_SIZE-1] != '\0')
	{
		/* error msg */
		errMsg(ERR_NUM);
		goto EXIT_PROC;
	}
#endif
	printf("Num 2 = ");
#if DEBUG
	strcpy(cArrNum_2, "-123456789123");
	puts("" );
#else
	scanf("%s", cArrNum_2);
	if(cArrNum_2[DATA_SIZE-1] != '\0')
	{
		/* error msg */
		errMsg(ERR_NUM);
		goto EXIT_PROC;
	}
#endif


	/* Num1 x num2 */
	iRet = MyMul(cArrNum_1, cArrNum_2, cArrRes);
	if(iRet)
	{
		/* error msg */
		errMsg(iRet);
		goto EXIT_PROC;
	}
	/* echo res */
	printf("%s x %s = %s", cArrNum_1, cArrNum_2, cArrRes);

EXIT_PROC:

	return EXIT_SUCCESS;
}

void errMsg(int iMsgNo)
{
	switch(iMsgNo)
	{
	case ERR_NUM_128:
		puts("ERROR: NUM > 128bit");
		break;
	case ERR_NUM_0_9:
		puts("ERROR: NUM = 0~9 ?");
		break;
	case ERR_OVERFLOW:
		puts("ERROR: NUM1 x NUM2 = OVERFLOW");
		break;
	default:
		puts("ERROR: unkown Error code");
	}
}


