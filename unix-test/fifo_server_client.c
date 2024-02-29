#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"pid_printf.h"

#define FIFO_PATH "fifofile.txt"

void server()
{
	char buf[256]={0};
	int fd;
	int count,status;

	//create a FIFO
	if(mkfifo(FIFO_PATH,0600))
	{
		perror("mkfifo");
	}
	pid_printf("The server is listening on fifofile.txt.\n");
	//open with block mode,will block until someone else connects to write
	
	fd=open(FIFO_PATH,O_RDONLY);
	pid_printf("client has connected.\n");
	//read data from FIFO
	while(1)
	{
		count=0;
		while(count<(sizeof(buf)-1))
		{
			status=read(fd,buf+count,1);
			if(status<1)
			{
				pid_printf("read EOF,parent exiting\n");
				goto parent_exit;
			}
			if(buf[count]=='\n')
			{
				break;
			}
			count++;
		}
		pid_printf("Recceive mess: %s",buf);
	}
	parent_exit:
		close(fd);
		unlink(FIFO_PATH);

}

void client(char **argv)
{
	int fd;
	char buf[256];
	//open FIFO for write
	fd=open(argv[1],O_WRONLY);
	pid_printf("The client is ready,Enter message(Ctrl + D to exit).\n");
	while(fgets(buf,sizeof(buf),stdin)!=NULL)
	{
		write(fd,buf,strlen(buf));
	}
	pid_printf("Client exiting.\n");
	close(fd);

}

int main(int argc,char **argv)
{
	if(argc<2)
	{
		server();
	}else
	{
		client(argv);
	}

}



