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
#include <string.h>
int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	char buf[88] = { '\0' };
	/*  入参检查 */
	if (argc < 2)
	{
		puts("using: ./myCat filename");
		return -1;
	}
	/*   打开文件 */
	if ((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("fopen");
		return -1;
	}
	/*  读文件 */
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		printf("%s", buf);
		fflush(stdout);
	}

	/*  关闭文件 */
	fclose(fp);

	return EXIT_SUCCESS;
}
