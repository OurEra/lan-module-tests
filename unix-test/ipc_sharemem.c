#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SEGSIZE 100

//写内存封装
void writeshm(int shmid,char *segptr,char *text)
{
	strcpy(segptr,text);
	printf("Done...\n");
}

//读内存封装
void readshm(int shmid,char *segptr)
{
	printf("segptr: %s\n",segptr);

}

void removeshm(int shmid)
{
	shmctl(shmid,IPC_RMID,0);
	printf("Shared memory segment marked for deletion\n");
}

void changemode(int shmid,char *mode)
{
	struct shmid_ds myshmds;
	//get current values for internal data structure
	shmctl(shmid,IPC_STAT,&myshmds);
	//display old permissions
	printf("old permissions were: %o\n",myshmds.shm_perm.mode);
	//convert and load the mode
	sscanf(mode,"%o",&myshmds.shm_perm.mode);
	//update the mode
	shmctl(shmid,IPC_SET,&myshmds);
	printf("New permissions are: %o\n",myshmds.shm_perm.mode);
}

void usage() //无参时的使用说明
{
	fprintf(stderr,"shmtol - A utility for tinkering with shared memory\n");
	fprintf(stderr,"USAGE: shmtool (w)rite <text>\n");
	fprintf(stderr,"(r)ead\n");
	fprintf(stderr,"(d)elete\n");
	fprintf(stderr,"(m)ode change <octal mode>\n");
	exit(1);
}

int main(int argc,char **argv)
{
	key_t key;
	int shmid,cntr;
	char *segptr;
	if(argc==1)usage();
	
	//Create unique key via call to ftok()
	key=ftok(".",'S');//申请一个指定的key
	
	//Open the shared memory segment - create if necessary
	if((shmid=shmget(key,SEGSIZE,IPC_CREAT|IPC_EXCL|0666))==-1)//申请一个指定key的共享内存，并判断有否指定的共享内存
	{
		printf("Shared memory segment exists - opening as client\n");
		//Segment probably already exists - try as a client
		if((shmid=shmget(key,SEGSIZE,0))==-1)  //如果已经有了指定的共享内存，则就用这个，并获取shmid号
		{
			perror("shmget");
			exit(1);
		}
	}else
	{
		printf("Creating new shared memory segment id = %d\n",shmid);
	}
	
	//attach (map) the shared memory segment into the current process
	if((segptr=(char*)shmat(shmid,NULL,0))==(void*)-1) //映射内存，连接内存
	{
		perror("shmat");
		exit(1);
	}	
	
	switch(tolower(argv[1][0]))
	{
		case 'w':
			writeshm(shmid,segptr,argv[2]);
			break;
		case 'r':
			readshm(shmid,segptr);
			break;
		case 'd':
			removeshm(shmid);
			break;
		case 'm':
			changemode(shmid,argv[2]);
			break;
		default:
			usage();
	}

}


