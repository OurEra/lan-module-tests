//P143_4-6文件锁练习，主进程加写锁，子进程打开时阻塞，等解锁后退出

#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//#include"pid_printf"
void pid_printf(char *format,...)//自定义printf,在每行输出前插入进程ID
{
	va_list ap;
	va_start(ap,format);
	printf("[%d]:",getpid());//每行前插入
	vprintf(format,ap);
	fflush(stdout);
}


//#include"filelock.h"

/* fcntl()简介
int fcntl(int fd,int cmd,struct flock *lock)  (文件符，文件锁，锁的结构体)
	cmd参数	F_GETLK	//获取锁的具体信息
	F_SETLK	//加锁，如果被拒绝，func出错返回
	F_SETLKW  //加锁阻塞形，如果加锁被拒绝，则阻塞，直到可以加锁
struct flock	//锁的结构体
{
	...
	short l_type;	//锁类型  F_RDLCK读锁，F_WRLCK写锁 , F_UNLCK解锁
	short l_whence;	//加锁文件偏移用的基点,可指定中间一片区域加锁 ， SEEK_SET ,SEEK_CUR, SEEK_END
	off_t l_start;	//相对基点的偏移量
	off_t l_len;	//文件长度多少字节  =0表示文件从起点到结束区域
	pid_t l_pid;	//加锁文件的ID
	...
	
}//*/

int set_lock(int fd,int cmd,int type,off_t offset,int whence,off_t len)//封装加解锁func
{
	struct flock lock;
	lock.l_type=type;
	lock.l_start=offset;
	lock.l_whence=whence;
	lock.l_len=len;
	return (fcntl(fd,cmd,&lock));
	//int fcntl(int fd,int cmd,struct flock *lock)  (文件符，文件锁，锁的结构体)
}

//进一步简化使用步骤，使用宏进行简化
//加读锁
#define read_lock(fd,offset,whence,len) set_lock(fd,F_SETLK,F_RDLCK,offset,whence,len)
//阻塞加读锁
#define readw_lock(fd,offset,whence,len) set_lock(fd,F_SETLKW,F_RDLCK,offset,whence,len)
//加写锁
#define write_lock(fd,offset,whence,len) set_lock(fd,F_SETLK,F_WRLCK,offset,whence,len)
//阻塞加写锁
#define writew_lock(fd,offset,whence,len) set_lock(fd,F_SETLKW,F_WRLCK,offset,whence,len)
//解锁
#define un_lock(fd,offset,whence,len) set_lock(fd,F_SETLK,F_UNLCK,offset,whence,len)

#define FILENAME "flock_test.txt"

//检测加锁是否成功
pid_t test_lock(int fd,int type,off_t offset,int whence,off_t len)
{
	struct flock lock;
	lock.l_type=type;
	lock.l_start=offset;
	lock.l_whence=whence;
	lock.l_len=len;
	if(fcntl(fd,F_GETLK,&lock)==-1)//先如果加锁被拒绝，显示出错，并返回-1
	{
		perror("fcntl");
		return -1;
	}

	//上一行没有被return -1掉，那说明加锁没被拒绝，则检查是否加锁成功，如果还是 解锁 态,则返回0
	if(lock.l_type==F_UNLCK)
	{
		return 0;
	}
	
	return lock.l_pid;	//没拒绝，也不是解锁态，那说明加锁成功，返回文件描述符
}

///////////////////////////
int main(int argc,char **argv)
{
	int fd;
	pid_t pid;
	pid_t lpid;
	
	if((fd=creat(FILENAME, 0640))==-1)//创建一个文件,并判断是否成功
	{
		perror("create flock_test.txt error!\n");
	}
	
	pid_printf("flock_test.txt created , fd = %d\n",fd);

	pid_printf("lock file flock_test.txt now\n");
	write_lock(fd,0,SEEK_SET,0);//加写锁
	if((pid=fork()) < 0)//派生子进程，并判断是否成功
	{
		printf("error fork process!\n");
	}else if(pid==0)
	{
		//printf("-Child process Code-\n");//子进程代码
		if( (fd=open(FILENAME, O_RDWR)) ==-1)
		{
			perror("open flock_test.txt error!\n");
		}
		while(1)
		{	
			lpid=test_lock(fd,F_RDLCK,0,SEEK_SET,0);
			if(lpid > 0)
			{
				pid_printf("process %d locked flock_test.txt\n",lpid);
			}else if(lpid==0)
			{
				pid_printf("flock_test.txt unlocked\n");
				break;
			}else
			{
				break;
			}
			sleep(1);
		}
		close(fd);
	}else
	{
		sleep(3);
		//printf("-Parrent Process Code-\n");//主进程代码
		pid_printf("unlock write lock of flock_test.txt now\n");
		un_lock(fd,0,SEEK_SET,0);//解锁
		wait(NULL);
	}
	close(fd);
	return 0;
}
