/*
 ============================================================================
 Name        : child.c
 Author      : root
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define N 1024	/* 限制最大1024个*/
#define DEBUG 0
typedef struct __node
{
	int num;
	struct __node *next;
}Lnode, *Linklist;

Linklist Creat(int);
Lnode* ChildCount(Linklist, int, int);
void prt(Lnode*);

#if DEBUG
void prtAll(Linklist L); /* debug */
void delAll(Linklist L); /* debug */
#endif

/*		int Totle	2~ 1024
 * 		int iNo		1 ~ 1024
 * 		int iNum	2 ~ 1024
 *		return:		1 ~ 1024
 */
int main(int argc, char *argv[])
{
	/* 初始化*/
	int iTotle	= 17;
	int iNo		= 3;
	int iNum	= 7;
	//Linklist L	= NULL;
	/* 入参检查 */
	if(argc != 4)
	{
		puts("\"./child Totle No Num\"   (Totle = 2~N, No = 1~N, iNum = 2~N, N < 1024)");
		printf("Default: #./child %d %d %d\n", iTotle, iNo, iNum);
	}else
	{
		iTotle 	= atoi(argv[1]);
		iNo		= atoi(argv[2]);
		iNum	= atoi(argv[3]);
		/* 入参合法性检查 */
		if(iTotle < 2 || iTotle > N || iNo > iTotle || iNum < 2 || iNum > N)
			puts("\"./child Totle No Num\"   (Totle = 2~N, No = 1~N, iNum = 2~N, N < 1024)");
	}
	printf ("%d个人围成圈，从第%d个人开始数，数到%d的人出列\n", iTotle, iNo, iNum);
	/* 计算 */
	prt(ChildCount(Creat(iTotle), iNo, iNum));
	//L = Creat(iTotle);
	//prtAll(L);
	//delAll(L);
	return EXIT_SUCCESS;
}
Lnode* ChildCount(Linklist L, int iNo, int iNum)
{
	int i = 0;
	Lnode *temp = NULL;
	/*  跳过iNo前几个小孩*/
	for(; L->num != iNo; L = L->next)
		;	/*  空语句 */
	/*  数到iNum的小孩退出 */
	for(; L->next != L; L = L->next, i++)
	{
		if(i == iNum - 1 - 1) /* 本来从自已数起，数2个小孩，要少移一格， 又因为链表需到前一个结点就停下，又要少移一格*/
		{
			i = 0; /* i 复位，等下次重数*/
			temp = L->next; /* 保存待删除的结点 */
			L->next = L->next->next;
			free(temp);
			L = L->next; /* 移到下一个起始点 */
		}
	}
	return L;
}

Linklist Creat(int iTotle)
{
	int i = 0;
	Linklist L	= NULL;
	Lnode *new	= NULL;
	Lnode *last	= NULL;	/* 尾指针  */
	/*  为了方便 ，先创建一个头 */
	L = (Lnode*)malloc(sizeof(Lnode));
	last = L;
	/* 尾插 */
	for(i = 1; i <= iTotle; i++)
	{
		 new = (Lnode*)malloc(sizeof(Lnode));
		 new->num = i;	/* 小孩编号 */
		 last->next = new;
		 last = new;
	}
	last->next = L->next; /*头尾相连，循环链表 */
	/* 删除头结点 */
	new = L;
	L = L->next;
	free(new);
	/* 返回链表 */
	return L;
}
void prt(Lnode *ptrNode)
{
	printf("Last child num = %d", ptrNode->num);
	free(ptrNode);	/* 删除最后的结点 */
}

#if DEBUG
void prtAll(Linklist L)
{
	Lnode *frist = L;	/* 记下环形链表的头指针 */
	printf("%3d", L->num); /* 先显示，并移动一格 */
	L = L->next;
	while(L != frist) /* 如果再指向头指针，则结束一圈显示 */
	{
		printf("%3d", L->num);
		L = L->next;
	}
}
void delAll(Linklist L)
{
	Lnode *frist = L;	/* 记下环形链表的头指针 */
	Lnode *temp = NULL;

	temp = L;
	L = L->next;
	free(temp);
	while(L != frist) /* 如果再指向头指针，则结束一圈 */
	{
		temp = L;
		L = L->next;
		free(temp);
	}
}
#endif
