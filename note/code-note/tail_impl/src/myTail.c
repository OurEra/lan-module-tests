/*
 ============================================================================
 Name        : myTail.c
 Author      : root
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 512
#define DEFAULT_LINE 10
enum ERR
{
	SUCCESS,
	ARG_ERROR,
	FILE_OPEN_ERROR
};
int main(int argc, char *argv[])
{
	char *strFilename	= NULL;
	FILE *fp			= NULL;
	char cBuf[BUF_SIZE]	= {0};
	int iLine			= DEFAULT_LINE;
	int iLineMax			= 0;
	/* 入参检查 */
	if(argc < 2)
	{
		puts("using ./myCat filename");
		return ARG_ERROR;
	}
	if(argc < 3)
	{
		strFilename = argv[1];	/* 只有2个参数，则第2参为文件名 */
	}else
	{
		iLine = atoi(argv[1]);	/* 否则 取第2参为行数，第3参为文件名 */
		iLine = iLine > 0 ? iLine : (0 - iLine);	/* 取正数 */
		strFilename = argv[2];
	}
	/* 打开文件 */
	fp = fopen(strFilename, "r");
	if(fp == NULL)
	{
		perror("myCat");
		return FILE_OPEN_ERROR;
	}
	/* 计数一共有多少行 */
	while(fgets(cBuf, sizeof(cBuf), fp) != NULL)
	{
		iLineMax++;
	}
	rewind(fp);		/* 文件指针 复位 */
	/*  跳过前面的行 */
	if(iLineMax > iLine)	/* 总计行数 比 入参的行数大，则不必跳过，直接显示*/
	{
		for(iLineMax -= iLine; fgets(cBuf, sizeof(cBuf), fp) != NULL && iLineMax > 0; iLineMax--)
		;	/*  空语句 */
	}
	/* 读一行，显示一行 */
	for(; fgets(cBuf, sizeof(cBuf), fp) != NULL && iLine > 0; iLine--)
	{
		printf("%s", cBuf);
	}
	/* 关闭文件*/
	fclose(fp);
	return SUCCESS;
}
