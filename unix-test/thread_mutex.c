#include<stdio.h>
#include<pthread.h>
//gcc xxx.c -lpthread
//用互斥锁mutex，来控制，轮流使用一个独占资源
//但等待的线程需要定时的去扫描资源是否可用

char buffer[128];
int buffer_has_data=0;
pthread_mutex_t mutex;//锁定标志 

void write_buffer(char *data)
{
	pthread_mutex_lock(&mutex);//锁定
	//pthread_mutex_trylock(&mutex);//锁定，可以避免死锁
	if(buffer_has_data==0)
	{
		sprintf(buffer,"%s",data);
		buffer_has_data=1;
	}
	pthread_mutex_unlock(&mutex);//解锁
}

void read_buffer(void)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);//锁定
		
		if(buffer_has_data==1)
		{
			printf("read buffer,data = %s\n",buffer);
			buffer_has_data=0;
		}
		pthread_mutex_unlock(&mutex);//解锁了 
		sleep(1);
	}
}

int main(int argc,char **argv)
{
	char input[128];
	pthread_t reader;
	pthread_mutex_init(&mutex,NULL);//生成互斥锁mutex,用默认属性初始化一个互斥锁对象
	pthread_create(&reader,NULL,(void*)(read_buffer),NULL);//(标识符,线程属性<一般不设为NULL>,运行的函数地址,函数的参数)
	while(1)
	{
		scanf("%s",input);
		write_buffer(input);
	}
	
	pthread_mutex_destroy(&mutex);//释放互斥锁mutex资源
	
	return 0;
}
