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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
int main(int argc, char *argv[])
{
	int fd;
	ssize_t n;
	char buf[88] = { '\0' };
	/*  入参检查 */
	if (argc < 2)
	{
		puts("using: ./myCat filename");
		return -1;
	}
	/*   打开文件 */
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		return -1;
	}
	printf("fd = %d\n", fd);

	/*  读文件 */
	while ((n = read(fd, buf, sizeof(buf) - 1)) != 0)/*n = 0代表文件结束*/
	{
		if (n < 0) /* n < 0 代表读出错*/
		{
			if (errno == EINTR) /* 但如果是被中断打断，则继续*/
			{
				continue;
			}
			perror("read");
			break;
		}
		buf[n] = '\0'; /*read不会自动填结束符*/
		printf("%s", buf); /*不加\n, 保证显示原有内容*/
		fflush(stdout); /*printf没有\n则因为缓存不即显示，所以强制输出*/
	}

	/*  关闭文件 */
	close(fd);

	return EXIT_SUCCESS;
}
