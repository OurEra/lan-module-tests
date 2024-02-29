#include	<stdio.h>
#include	<signal.h>

main()
{
   void	func1(int);
   void  func2(int);
   int	i;
   signal( SIGINT, func1);	
   signal( SIGQUIT, func2);		
   for(i=0; i<5; i++ ){		
	printf("hello\n");
	sleep(1);
      }
}

void func1(int signum)		
{
  int i=0;
  for(i=0;i<10;i++)
        {
	  printf("%d\n",i);
          sleep(1);
         }
}

void func2(int signum)		
{
  int i=0;
  for(i=10;i<20;i++)
        {
	  printf("%d\n",i);
     sleep(1);
         }
}
