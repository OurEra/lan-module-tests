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
	while (fread(buf, sizeof(buf) - 1, 1, fp) > 0)/* 是否充满BUF */
	{
		/*  显示文件 */
		//buf[sizeof(buf) - 1] = '\0';  /* BUF声明就全清0过了，些处不需重复清0*/
		printf("%s", buf);
		fflush(stdout);
		memset(buf, '\0', sizeof(buf));
		/*  fread不能返回读取了多少个字符，
		 * 所以最后一次数据无法定位在哪里填'\0'
		 * 不得以的办法，每次显示完以后，把BUF全清空，
		 * 这样才能保证最后一次充不满BUF的情况下，数据后面有结束符，
		 * *** ‘\0’仅仅是为了printf和strcpy类所使用，
		 * 如果不用printf或是strcpy和'\0'相关的，则不爱和填'\0',
		 * 可以参考书关于 cp实现的代码，就不需填'\0'   */
	}
	printf("%s", buf); /*显示 最后一次 没有充满BUF 的数据*/
	fflush(stdout);

	/*  关闭文件 */
	fclose(fp);

	return EXIT_SUCCESS;
}
