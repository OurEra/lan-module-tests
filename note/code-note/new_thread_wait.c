#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<linux/unistd.h>
#include<errno.h>

void thread()
{
	int i;
	for(i=0;i<3;i++)
	{
		printf("This is a pthread.\n");
		sleep(1);
	}
	pthread_exit((void*)i);//线程退出，并返回代码
}

int main()
{
	pthread_t id;
	int i,ret;
	void *result;
	ret=pthread_create(&id,NULL,(void*)thread,NULL);//(标识符,线程属性<一般不设为NULL>,运行的函数地址,函数的参数)
	if(ret!=0)
	{
		printf("Create prhread error!\n");
		exit(1);
	}
	for(i=0;i<3;i++)
	{
		printf("This is main proess.\n");
		sleep(1);
	}
	pthread_join(id,&result);//等待id号的线程退出，并返回线程的退出代码
	printf("Child thread return %d\n",(int)result);
	return (0);	
}
