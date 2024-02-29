//sigdemo4.c
//演示信号如何安装函数
#include	<stdio.h>
#include	<signal.h>

main()
{
	void	f(int);
	int	i;
	signal( SIGUSR2, f );
	kill(getpid(), SIGUSR2);
	for(i=0; i<5; i++ ){		
		printf("hello\n");
		sleep(1);
	}
}

void f(int signum)		
{
	printf("Catch a CTRL-C signal!!!\n");
}
