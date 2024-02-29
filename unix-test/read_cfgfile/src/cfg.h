/*
 * cfg.h
 *
 *  Created on: 2011-3-11
 *      Author: root
 */

#ifndef CFG_H_
#define CFG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 100

typedef struct _cfg
{
	char key[64];
	char item[64];
}cfg;


typedef struct _node
{
	cfg data;
	struct _node *next;
}cfgNode, *cfgList;


typedef struct _acc
{
	int perm;
	char user[32];
	char pswd[32];
}acc;

typedef struct _accNode
{
	acc data;
	struct _accNode *next;
}accNode, *accList;

void readToList(char*);	/* 从文件读数据成链表 */
void ExitFree();			/* 程序结束时回收资源 */
char* getItem(char*);		/*  获取CFG对应key的数据 */
void PrtInfo();			/* 仅测试时打印数据内容 */

#endif /* CFG_H_ */
