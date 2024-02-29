//进程信号量控制

/* 
//创建一个信号量集(信号键值，信号量数目，信号标志)
//标志IPC_CREAT:如果key指定的信号量不存在，就创建一个信号量集
//标志IPC_EXCL:和 IPC_CREAT标志一起使用，如果信号量已经存在，返回错误
//semget()成功时，返回与键值对应的信号量集的标识符，否则返回-1
int semget(key_t key,int nsems,int semflg)

//信号量操作(信号量ID，sembuf结构数组，sops指向数组的长度)
//semop()调用成功时返回0，失败返回-1
//如果信号量sem_op<0，则这个操作会导致进程阻塞，直到信号量的值至少等于筥作值的绝对值
int semop(int semid,struct sembuf *sops,unsigned nsops)

//semop()的sembuf结构数组
struct sembuf
{
	unsigned short	sem_num; //信号量索引号， 0对应第一个信号量，1对应第二个信号量，依此类推
	short		sem_op;	//信号量操作值，=0会使进程阻塞，>0则加上信号量， <0则减信号量
	short		sem_flg;//符号位,指定IPC_NOWAIT以防止操作阻塞，如果该操作本应阻塞，则semop调用会失败，
				//如果指定SEM_UNDO,linux会在进程出错退出时，自动撤销该次操作
               

//信号量控制(信号量集，对哪个信号进行操作，具体操作类型,XXXX)，最后一个参数一船、般为NULL或0
//CMD:各种操作类型
//IPC_STAT获取信号量信息，并由arg.buf返回，记录在semid_ds结榴弹体
//IPC_SET设置信号量信息，待设置信息保存在arg.buf中
//IPC_RMID立即删除信号量集
//IPC_INFO获取信号量集信息，由arg.buf返回,记录在一个struct seminfo结构体中
//SEM_INFO返回和IPC_INFO一样的seminfo结构体,只是部分字段含义有所区别
//SEM_STAT回和IPC_STAT一样的sem_ds结构体,只是部分字段含义有所区别
//GETALL返回所有信号量的值,结果保存在arg.array中,参数senum被忽略
//GETNCNT返回等待semnum所代表信号量的值增加的进程数,相当于目前有多少进程在等待semnum所代表的共享资源
//GETPID返回最后一个对semnum所代表信号量执行semop操作的进和ID
//GETVAL返回semnum所代表信号量的值；GETZCNT返回等侍semnum所代表信号量的值变成0的进程数
//GETZCNT返回等待semnum所代表的信号量成为0的进程数目
//SETALL通过arg.array更新所有信号量的值，同时，更新与本信号集丰关的semid_ds结构的sem_ctime成员
//SETVAL设置semnum所代表信号量的值为arg.val
//semctl()调作失败时返回-1，调用成功时，跟据CMD值不同，返回值也不同，具体看semctl man手册页
int semctl(int semid,int semnum,int cmd,union semun arg);

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<sys/shm.h>
#include"pid_printf.h"

#if defined(__GNU_LIBRARY__)&&!defined(_SEM_SEMUN_UNDEFINED)

#else
union semun
{
	int val;//SETVAL使用的值
	struct semid_ds *buf;//buffer for IPC_STAT,IPC_SET
	unsigned short *array;//array for GETALL,SETALL
	struct seminfo *__but;//buffer for IPC_INFO(Linux specific)
};
#endif

int semheld=0;
int master=0;
int id=0;

//pid_printf()


////////////////

//释放信号量
void sem_release(int id)
{
	struct sembuf sb;//建一个用于操作的结构
	if(semheld<1)
	{
		pid_printf("I don't have any reources;nothing to release\n");
		return;
	}
	sb.sem_num=0; //信号索引(因为是信号数组，可以有N多个信号，0指定为对第一个信号设置)
	sb.sem_op=1;	//信号量
	sb.sem_flg=SEM_UNDO;	//操作标志位，给系统内核核用，如果信号死掉，则系统内核撤消，防止死锁
	if(semop(id,&sb,1)==-1)//把输入ID的信号量，按上面的结构数据进行设置
	{
		pid_printf("Semop release error: %s\n",strerror(errno));
		exit(-1);
	}
	semheld--;
	pid_printf("Resource released.\n");
}

//请求信号量
void sem_request(int id)
{
	struct sembuf sb;
	if(semheld>0)
	{
		pid_printf("I already hold the resoure;not requesting another one.\n");
		return;
	}
	sb.sem_num=0;
	sb.sem_op=-1;
	sb.sem_flg=SEM_UNDO;
	pid_printf("Requesting resource ...");
	fflush(stdout);//清空缓冲区
	if(semop(id,&sb,1)==-1)//如果这个-1操作会把sem_op减的<0，就会使进程阻塞，直到信号量至少等于操作值的绝对值
	{
		pid_printf("Semop request error: %s\n",strerror(errno));
		exit(-1);
	}
	semheld++;
	pid_printf("Done.\n");
}

//删除信号量
void sem_delete(void)
{
	printf("Master exiting;delete semaphore.\n");
	if(semctl(id,0,IPC_RMID,0)==-1)
	{
		pid_printf("Error releasing semphore.\n");
	}
}


//////////////
int main(int argc,char **argv)
{
	union semun sunion;
	if(argc<2)
	{
		if((id=semget(IPC_PRIVATE,1,SHM_R|SHM_W))!=-1)//申请一个信号，并判断是否成功
		{
			atexit(&sem_delete);//进程退出时，自动调用释放信号
			sunion.val=1;//信号量初始化为1
			if(semctl(id,0,SETVAL,sunion)==1)//初始设置信号量
			{
				pid_printf("semctl failed: %s\n",strerror(errno));
				exit(-1);
			}
		}
		master=1;
	}else
	{
		id=atoi(argv[1]); //将字符串转换成整型数
		pid_printf("Using existing semaphore %d.\n",id);
	}
	
	if(id==-1)
	{
		pid_printf("Semaphore request failed: %s \n",strerror(errno));
		return 0;
	}
	
	pid_printf("Successfully allocated semaphore id %d.\n",id);//申请成功，则显示ID

	while(1)
	{
		int action;
		printf("\nStatus: %d request held by this process.\n",semheld);
		
		printf("Please select:\n");
		printf("1.Release a resource\n");
		printf("2.Request a resource\n");
		printf("3.Exit this process\n");
		printf("Your choice:");
		
		scanf("%d",&action);
		
		switch(action)
		{
			case 1:
				sem_release(id);
				break;
			case 2:
				sem_request(id);
				break;
			case 3:
				exit(0);
				break;
		}
	}
	return 0;
}
