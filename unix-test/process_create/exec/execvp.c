#include <unistd.h>
#include <stdio.h>

/*this function illustrate execvp usage*/
int main(int argc, char **argv)
{
  char *arglist[3];
  arglist[0] = "ls";
  arglist[1] = "-l";
  arglist[2] = NULL;
  printf("***************exec ls -l****************\n");
  if (execvp("ls", arglist) == -1) {
    perror("ececvp error");
  }
  printf("*************ls -l  finished!************\n");
}
