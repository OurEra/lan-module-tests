#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/wait.h>

/*this program illustrate waitpid function*/
int main(int argc, char **argv)
{
  int status;			//waitpid second parameter
  pid_t pid = fork();
  if (pid == 0) {
    //child process part
    printf("Child process will sleep 5 seconds.\n");
    sleep(5);
    printf("child  PID=[%d] will exit with status 3.\n", getpid());
    exit(3);
  } else if (pid != -1) {
    //parent process part
    pid_t pid_c;
    do {
      pid_c = waitpid(pid, &status, WNOHANG);
      if (pid_c == 0) {
	printf("No child process exit\n");
	sleep(1);
      }
    } while (pid_c == 0);
    if (WIFEXITED(status)) {
      /*如果WIFEXITED返回非零值 */
      printf("the child process %d exit nomlly.\n", pid_c);
      printf("the return code is %d.\n", WEXITSTATUS(status));
    } else {
      /*如果WIFEXITED返回零 */
      printf("the child process %d exit abnomlly.\n", pid_c);
    }
  } else {
    printf("There was an error with forking!\n");
  }
  return 0;
}
