#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>
//用信号量，来控制3个线程运行的先后顺序

sem_t sem1,sem2;

void *thread_a(void*in)
{
	printf("Thread_a run\n");
	sem_post(&sem2);//增加信号sem2,让thread_b可以运行
}

void *thread_b(void*in)
{
	sem_wait(&sem2);//sem2＝0时阻塞，等待同步信号
	printf("Thread_b run\n");
	sem_post(&sem1);//增加信号sem1,让thread_a可以运行
}

void *thread_c(void*in)
{
	sem_wait(&sem1);//sem1＝0时阻塞，等待同步信号sem1
	//sem_trywait(&sem1);//与上面的区别在 sem1＝0时不阻塞，等待同步信号sem1
	printf("Thread_c run\n");//无阻塞,直接运行
	
}

int main(int argc,char **argv)
{
	pthread_t a,b,c;
	//线程同步信号
	sem_init(&sem1,0,0);//初始化信号量（信号量变量,=0时当前进程中所有线程共享!=0进程共享,信号量初始值）
	sem_init(&sem2,0,0);

	//创建3个子线程
	pthread_create(&a,NULL,thread_a,NULL);//(标识符,线程属性<一般不设为NULL>,运行的函数地址,函数的参数)
	pthread_create(&b,NULL,thread_b,NULL);
	pthread_create(&c,NULL,thread_c,(void*)0);// (void*)0等价NULL,不同写法，意义一样

	//主线程等待线程退出
	pthread_join(a,NULL);
	pthread_join(b,NULL);
	pthread_join(c,NULL);

	//释放信号量资源
	sem_destroy(&sem1);
	sem_destroy(&sem2);

	return 0;
}
