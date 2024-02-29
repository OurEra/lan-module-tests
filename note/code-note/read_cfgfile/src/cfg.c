/*
 * cfg.c
 *
 *  Created on: 2011-5-14
 *      Author: root
 */

#include "cfg.h"

/* 不需要给外面调用的函数，用static限制其作用域 */
static FILE* openFile(char*, char*);
static void closeFile(FILE*);
static void insertAccNode(char*);	/* 链表插入帐户数据 */
static void insertCfgNode(char*);	/* 链表插入配置数据 */
static void delSpace(char *);		/* 去空格  */

/* 在单进程单线程中，为了方便操作，可用全局变量，
 * 在多进程，多线程中，尽可能不使用全局变量
 * 尽量养成不使用全局变量的习惯
 */
/* 不需要给外面调用的全局变量，用static限制其作用域 */
static cfgList cL = NULL;
static accList aL = NULL;

void readToList(char *strFilename)
{
	void(*ptrFun)(char *) = NULL;
	FILE *fp = NULL;
	char strBuf[BUF_SIZE] = {0};
	char *ptrStr = NULL;
	int i = 0;
	fp = openFile(strFilename, "r");
	if(fp == NULL)
	{
		return;
	}
	while(fgets(strBuf, BUF_SIZE, fp) != NULL)
	{
		for(ptrStr = strBuf; *ptrStr == ' ' && *ptrStr != '\0';ptrStr++); /* 跳过开头的空格 */
		switch(*ptrStr)
		{
			case '\r':
			case '\n':
			case ';':
			case '#':break;
			case '[':/* 为了方便识别，先转成大写 */
					for(i = 0; *(ptrStr + i) != '\n'; i++)
					{
						*(ptrStr + i) = toupper(*(ptrStr + i));
					}
					/*  一并去除后面的空格和\n，主要是最后的空格 */
					delSpace(ptrStr);
					if(strcmp(ptrStr, "[CFG]") == 0)
					{	/*  函数指针 */
						ptrFun = insertCfgNode;
						break;
					}else if(strcmp(ptrStr, "[ACCOUNT]") == 0)
					{
						ptrFun = insertAccNode;
						break;
					}
					break;
			default:delSpace(ptrStr);
					ptrFun(ptrStr);	/* 由函数指针 引导处理方式 */
		}
	}
	closeFile(fp);
}

char* getItem(char *key)
{
	cfgNode *cTemp = cL;
	for(; cTemp != NULL; cTemp = cTemp->next)
	{
		if(strcmp(key, cTemp->data.key) == 0)
			return cTemp->data.item;
	}
	return NULL;
}

static void insertAccNode(char *Data)
{
	accNode *temp = NULL;
	if(aL == NULL)
	{
		aL = (accNode*)malloc(sizeof(accNode)); /* 生成头结点*/
		aL->next = NULL;
	}
	temp = (accNode*)malloc(sizeof(accNode));
	strtok(Data, "=/");/* 忽略第一个acc字符*/
	temp->data.perm = atoi(strtok(NULL, "=/\r\n"));
	strcpy(temp->data.user, strtok(NULL, "=/\r\n"));
	strcpy(temp->data.pswd, strtok(NULL, "=/"));
	/* 头插 */
	temp->next = aL->next;
	aL->next = temp;
}
static void insertCfgNode(char *Data)
{
	cfgNode *temp = NULL;
	if(cL == NULL)
	{
		cL = (cfgNode*)malloc(sizeof(cfgNode)); /* 生成头结点*/
		cL->next = NULL;
	}
	temp = (cfgNode*)malloc(sizeof(cfgNode));
	strcpy(temp->data.key, strtok(Data, "=\r\n"));
	strcpy(temp->data.item, strtok(NULL, "=\r\n"));
	/* 头插 */
	temp->next = cL->next;
	cL->next = temp;
}
static void delSpace(char *Data)
{
	char *ptr = Data;
	/*  找到后面的第一个空格或\n */
	for(; *ptr != '\0' && *ptr != ' ' && *ptr != '\n' && *ptr != '\r'; ptr++, Data++);
	/*  判断是否是有空格导致的for中断 */
	if(*ptr == '\0') return; /* 无空格返回*/
	/*  有空格则消空格 */
	for(; *Data != '\0' ; Data++)
	{
		if(*Data != ' ' && *Data != '\n' && *Data != '\r')
		{
			*ptr = *Data;
			ptr++;
		}
	}
	*ptr = '\0'; /* 最后填上结束符 */
}
void ExitFree()
{
	cfgNode *cTemp = NULL;
	accNode *aTemp = NULL;
	while(cL != NULL)
	{
		cTemp = cL;
		cL = cL->next;
		free(cTemp);
	}
	cL = NULL;
	while(aL != NULL)
	{
		aTemp = aL;
		aL = aL->next;
		free(aTemp);
	}
	aL = NULL;
}

void PrtInfo()
{
	cfgNode *cTemp = cL;
	accNode *aTemp = aL;
	aTemp = aTemp->next;
	while(aTemp != NULL)
	{
		printf("%-5d%-16s%-16s\n",aTemp->data.perm, aTemp->data.user, aTemp->data.pswd);
		aTemp = aTemp->next;
	}
	while(cTemp != NULL)
	{
		printf("%-10s%-10s\n",cTemp->data.key, cTemp->data.item);
		cTemp = cTemp->next;
	}
}
static FILE* openFile(char *strFilename, char *mode)
{
	FILE *fp = NULL;
	fp = fopen(strFilename, mode);
	if(fp == NULL)
	{
		perror("open");
		return NULL;
	}
	return fp;
}

static void closeFile(FILE *fp)
{
	if(fp != NULL)
	{
		fclose(fp);
	}
}
