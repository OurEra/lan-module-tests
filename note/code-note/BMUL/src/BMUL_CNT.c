/*
 * BMUL_CNT.c
 *
 *  Created on: 2011-3-6
 *      Author: root
 */

#include "BMUL_CNT.h"

static int NumCheck(const char*);
static void NumToStr(char*, int);
static void StrToNum(char*);
static void resNumToStr(char*, int);
static void count_bit_mul(char, char, char*);

int MyMul(char *arrNum1, char* arrNum2, char* arrRes)
{
	/* init */
	int 			iRet		= 0;
	unsigned int 	iResPtr		= 0;
	unsigned int 	iResLen		= BUF_SIZE -2;
	unsigned int 	iLenNum1	= 0;
	unsigned int 	iLenNum2	= 0;
	unsigned int		iLenRes		= 0;
	int 			iLoop1		= 0;
	int 			iLoop2		= 0;
	char* 			cpTemp		= NULL;

	/*  负数处理 */
	if(*arrNum1 == '-')
	{
		*arrRes = '-';	/* 结果存负值 */
		arrNum1++;		/* 跳过符号位 */
	}
	if(*arrNum2 == '-')
	{
		if(*arrRes == '-') *arrRes = '\0';
		else *arrRes = '-';
		arrNum2++;
	}
	if(*arrRes == '-')
	{
		arrRes++;
		iResLen--;
	}

	/*  去除符号位后的数据长度 */
	iLenNum1	= strlen(arrNum1);
	iLenNum2	= strlen(arrNum2);
	iLenRes	= iResLen;

	/* Num Check 0~9? */
	iRet = NumCheck(arrNum1);
	if(iRet) return iRet;
	iRet = NumCheck(arrNum2);
	if(iRet) return iRet;

	/* Str -> Num */
	StrToNum(arrNum1);
	StrToNum(arrNum2);

	/*
	 * Num1 x Num2
	 */
	if(iLenNum1 < iLenNum2) /* 保持被乘数大 ，乘数小 */
	{
		cpTemp 	= arrNum1 ;
		arrNum1 	= arrNum2;
		arrNum2 	= cpTemp;
		cpTemp	= NULL;
		iLenNum1 = iLenNum1 ^ iLenNum2;
		iLenNum2 = iLenNum1 ^ iLenNum2;
		iLenNum1 = iLenNum1 ^ iLenNum2;
	}
	/* Mul */
	for(iLoop2 = iLenNum2 - 1; iLoop2 >= 0; iLoop2--)
	{
		iResPtr = iLenRes--;
		if(iResPtr < 1) return ERR_OVERFLOW;
		for(iLoop1 = iLenNum1 - 1; iLoop1 >= 0; iLoop1--)
		{
			/*  2数相乘, 最大9*9＝81, */
			count_bit_mul(arrNum1[iLoop1], arrNum2[iLoop2], &arrRes[iResPtr]);
			iResPtr--;
		}
	}
	/* Num -> Str*/
	NumToStr(arrNum1, iLenNum1);
	NumToStr(arrNum2, iLenNum2);
	resNumToStr(arrRes, iResLen);

	return OK;
}

static void count_bit_mul(char cNum1, char cNum2, char* cRes)
{
	static char cTemp;
	static char cHigh;
	static char cLow;
	cTemp = cNum1 * cNum2;
	/*  个位 = (上次该位数 + 本次结果)的个位数 */
	cLow = (*cRes + cTemp)%10;
	/*  进位 =   (上次该位数 + 本次结果)的进位 */
	cHigh = (*cRes + cTemp)/10;
	/*  保存本次计算的个位 */
	*cRes = cLow;
	/*  上次位 + 本次进位 */
	*(cRes-1) += cHigh;
}

static int NumCheck(const char* arrNum)
{
	while(*arrNum != '\0')
	{
		if(*arrNum < '0' || *arrNum > '9') return ERR_NUM_0_9;
		arrNum++;
	}
	return OK;
}

static void NumToStr(char* arrNum, int iLen)
{
	for(; iLen > 0; iLen--,arrNum++) *arrNum += '0';
}

static void resNumToStr(char* arrNum, int iLen)
{
	char* cpHead 	= NULL;
	char* cpString = NULL;
	cpHead = arrNum;
	/* jump '\0' */
	while(*arrNum == '\0')
	{
		arrNum++;
		iLen--;
	}
	cpString = arrNum;
	for(; iLen >= 0; iLen--,arrNum++) *arrNum += '0';
	/* 存结果时，是从后往前存的， 所以移动数据占前面的空位*/
	while((*cpHead++ = *cpString++) != '\0')
	{
		;/* nop */
	}
}

static void StrToNum(char* arrNum)
{
	while (*arrNum != '\0') *arrNum++ -= '0';
}
