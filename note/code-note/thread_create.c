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
}

int main()
{
	pthread_t id;
	int i,ret;
	//(void*)0等价NULL,不同写法，意义一样
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
	pthread_join(id,NULL);//等待线程退出

	return 0;	
}
