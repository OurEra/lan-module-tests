/*
 ============================================================================
 Name        : FlieComp.c
 Author      : root
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int fcomp(char**);

int main(int argc, char* argv[])
{

	if (argc < 3)
	{
		puts("usage: ./mycomp file1 file2 ");
		return 0;
	}

	fcomp(argv);

	return EXIT_SUCCESS;
}

/* 在没有enum 型变量做错误错时， 用数字比较难看懂*/

int fcomp(char* argv[])
{
	int iRet = 0;
	int iLine = 0;
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	char cArrBuff1[BUF_SIZE] = { 0 };
	char cArrBuff2[BUF_SIZE] = { 0 };
	/*  文件打开存在？ */
	fp1 = fopen(argv[1], "r");
	if (fp1 == NULL)
	{
		system("pwd");
		printf("%s open error\n", argv[1]);
		fflush(stdout);
		iRet = -9;
	}
	fp2 = fopen(argv[2], "r");
	if (fp2 == NULL)
	{
		system("pwd");
		printf("%s open error\n", argv[2]);
		fflush(stdout);
		iRet = -9;
	}
	if (iRet != 0)
	{
		goto EXIT_PROC;
	}
	/* 读入 */
	while (1)
	{
		iLine++;
		if (fgets(cArrBuff1, sizeof(cArrBuff1) / sizeof(char), fp1) == NULL)
		{
			iRet = -1;
		}
		if (fgets(cArrBuff2, sizeof(cArrBuff1) / sizeof(char), fp2) == NULL)
		{
			iRet += -2;
		}
		if (cArrBuff1[sizeof(cArrBuff1) / sizeof(char) - 1] != '\0')
		{
			iRet = -7;
		}
		if (cArrBuff2[sizeof(cArrBuff2) / sizeof(char) - 1] != '\0')
		{
			iRet += -8;
		}
		if (iRet != 0)
		{
			break;
		}
		/* 比较*/
		if (strcmp(cArrBuff1, cArrBuff2) != 0)
		{
			/*  不相同 */
			if (cArrBuff1[0] != '\0')
				printf("file1:[%d] %s", iLine, cArrBuff1);
			if (cArrBuff2[0] != '\0')
				printf("file2:[%d] %s\n", iLine, cArrBuff2);
		}
	}
	/*  错误码显示 */
	switch (iRet)
	{
		case -1:
			puts("file1 End!\n");
			break;
		case -2:
			puts("file2 End!\n");
			break;
		case -3:
			puts("Complete!\n");
			break;
		case -7:
			printf("file1 Line overflow:[%d]\n", iLine);
			break;
		case -8:
		case -15:
			printf("file2 Line overflow:[%d]\n", iLine);
			break;
		default:
			puts("unknow error code!\n");
	}
	fflush(stdout);

EXIT_PROC:
	if (fp1 != NULL)
	{
		fclose(fp1);
	}
	if (fp2 != NULL)
	{
		fclose(fp2);
	}
	return iRet;
}
