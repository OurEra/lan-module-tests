/*
 ============================================================================
 Name        : C_test.c
 Author      : root
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "cfg.h"
#define DEBUG 0
#define FILE_NAME "ini.txt"
int main(void)
{
	readToList(FILE_NAME);

	printf("get ip = %s\n", getItem("ip"));
	printf("get port = %s\n", getItem("port"));
	printf("get log = %s\n", getItem("log"));

	PrtInfo();

	ExitFree();

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
