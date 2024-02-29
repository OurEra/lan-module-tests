#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
//include"message.h"

#define MAX_SEND_SIZE 80
struct mymsgbuf
{
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

void send_message(int qid,struct mymsgbuf *qbuf,long type,char *text)
{
	printf("Sending a message ... \n");
	qbuf->mtype=type;
	strcpy(qbuf->mtext,text);
	if(-1==(msgsnd(qid,(struct msgbuf*)qbuf,strlen(qbuf->mtext)+1,0)))
	{
		perror("msgsnd");
		exit(1);
	}
}

void read_message(int qid,struct mymsgbuf *qbuf,long type)
{
	printf("Reading a message ...\n");
	qbuf->mtype = type;
	msgrcv(qid,(struct msgbuf*)qbuf,MAX_SEND_SIZE,type,0);
	printf("Type: %ld  Text: %s\n",qbuf->mtype,qbuf->mtext);
}

void remove_queue(int qid)
{
	msgctl(qid,IPC_RMID,0);
}

void change_queue_mode(int qid,char *mode)
{
	struct msqid_ds myqueue_ds;
	msgctl(qid,IPC_STAT,&myqueue_ds);
	sscanf(mode,"%ho",&myqueue_ds.msg_perm.mode);
	msgctl(qid,IPC_SET,&myqueue_ds);
}


void usage(void)
{
	fprintf(stderr,"msgtool -A utility for tinkering with msg queues\n");
	fprintf(stderr,"USAGE:msgtool(s)end <type><messagetext>\n");
	fprintf(stderr,"(r)ecv <type>\n");
	fprintf(stderr,"(d)elete\n");
	fprintf(stderr,"(m)ode <octal mode>\n");
	exit(1);
}

int main(int argc,char **argv)
{
	key_t key;
	int msgqueue_id;
	struct mymsgbuf qbuf;
	if(argc==1)usage();

	key=ftok(".",'m');
	if(-1==(msgqueue_id=msgget(key,IPC_CREAT|0660)))
	{
		perror("msgget");
		exit(1);
	}
	printf("message queue id = %d\n",msgqueue_id);
	
	switch(tolower(argv[1][0]))// tolower转成小写
	{
		case's':	if(argc<4)
					{
						usage();
						break;
					}
					send_message(msgqueue_id,(struct mymsgbuf *)&qbuf,atol(argv[2]),argv[3]);  //atol 将int转成null型
					break;
		case'r':	if(argc<3)
					{
						usage();
						break;
					}
					read_message(msgqueue_id,&qbuf,atol(argv[2]));
					break;
		case'd':	remove_queue(msgqueue_id);
					break;
		case'm':	if(argc<3)
					{
						usage();
						break;
					}
					change_queue_mode(msgqueue_id,argv[2]);
					break;
		default: 	usage();
	}
	return(0);
}






