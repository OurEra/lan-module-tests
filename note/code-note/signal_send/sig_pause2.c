//sigdemo8.c
#include <stdio.h>
#include <signal.h>

void sigroutine(int unused)
{ 
 printf("Catch a signal SIGINT ");
} 
int main() 
{ 
  signal(SIGINT, sigroutine);
  alarm(3); 
  pause(); 
  printf("receive a signal ");
} 
