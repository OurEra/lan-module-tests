/*
 ============================================================================
 Name        : myCat.c
 Author      : root
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 512
enum ERR
{
	SUCCESS,
	ARG_ERROR,
	FILE_OPEN_ERROR
};

int main(int argc, char *argv[])
{
	FILE *fp 			= NULL;
	char cBuf[BUF_SIZE] = {0};
	/* 入参检查 */
	if(argc < 2)
	{
		puts("using ./myCat filename");
		return ARG_ERROR;
	}
	/* 打开文件 */
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		perror("myCat");
		return FILE_OPEN_ERROR;
	}
	/* 读一行，显示一行 */
	while(fgets(cBuf, sizeof(cBuf), fp) != NULL)
	{
		printf("%s", cBuf);
	}
	/* 关闭文件*/
	fclose(fp);
	return SUCCESS;
}
