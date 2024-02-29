#include<stdio.h>
#include<pthread.h>

//gcc xxx.c -lpthread
//用互斥锁mutex控制轮流使用一个独占资源时，
//用唤醒和阻塞来让下一个线程wait，因不需要定时扫描资源是否可用
//而提高了等待时的CPU效率

char buffer[128];
int buffer_has_data=0;
pthread_mutex_t mutex;//锁定标志 
pthread_cond_t cond;//唤醒条件变量

void write_buffer(char *data)
{
	pthread_mutex_lock(&mutex);//锁定
	if(buffer_has_data==0)
	{
		sprintf(buffer,"%s",data);
		buffer_has_data=1;
		pthread_cond_signal(&cond);//唤醒单个线程
		//pthread_cond_broadcast(&cond);//唤醒所有线程,test1,test2
	}

	pthread_mutex_unlock(&mutex);//解锁
}

void read_buffer(void) //
{
	while(1)
	{
		pthread_mutex_lock(&mutex);//锁定, 阻塞前被锁上
		while(!buffer_has_data)//无数据时,一般用循环体阻塞,这样可以判断当前条件是否满足唤醒,否则继续阻塞,这样在多线程相互作用时,比较稳定
		{
			pthread_cond_wait(&cond,&mutex);//阻塞线程,等待唤醒 和 解锁mutex, 注意有2个参数，第一个是唤醒变量cond,第2个是解锁mutex
		}
		printf("read buffer,data = %s\n",buffer);
		buffer_has_data=0;
		pthread_mutex_unlock(&mutex);//解锁
	}
}

test1()
{
	while(1)
	{
		pthread_cond_wait(&cond,&mutex);//阻塞线程,等待唤醒 和 解锁mutex, 注意有2个参数，第一个是唤醒变量cond,第2个是斥锁mutex
		printf("test1 run\n");
	}
	
}

test2()
{
	while(1)
	{
		pthread_cond_wait(&cond,&mutex);//阻塞线程,等待唤醒 和 解锁mutex, 注意有2个参数，第一个是唤醒变量cond,第2个是斥锁mutex
		printf("test2 run\n");
	}
}

int main(int argc,char **argv)
{
	char input[128];
	pthread_t reader,t1,t2;
	
	pthread_mutex_init(&mutex,NULL);//生成互斥锁mutex,默认属性初始化
	pthread_cond_init(&cond,NULL);//生成一个唤醒变量,默认属性=同一进程内的所有线程使用
	
	pthread_create(&reader,NULL,(void*)(read_buffer),NULL);//(标识符,线程属性<一般不设为NULL>,运行的函数地址,函数的参数)
	pthread_create(&t1,NULL,(void*)(test1),NULL);//测试一次唤醒多个线程
	pthread_create(&t2,NULL,(void*)(test2),NULL);//测试一次唤醒多个线程

	while(1)
	{
		scanf("%s",input);
		write_buffer(input);
	}
	
	pthread_join(reader,NULL);//主线程等待reader线程退出
	pthread_cond_destroy(&cond);//释放阻塞唤醒变量
	pthread_mutex_destroy(&mutex);//释放互斥锁mutex资源
		

	return 0;
}
