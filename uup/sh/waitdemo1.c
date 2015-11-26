#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define DELAY 5

void child_process(int delay);
void parent_process(int childpid);
void pr_exit(int status);

int main(int argc, char const *argv[])
{
  int newpid;

  printf("Before: pid is %d\n", getpid());

  if ((newpid = fork()) == 0)
  {
    child_process(DELAY);
  }
  else
  {
    parent_process(newpid);
  }
  return 0;
}

void child_process(int delay)
{
  printf("child %d here, parent is %d,  will sleep %d seconds\n", getpid(), getppid(), delay);
  sleep(delay);
  printf("child done. about to exit\n");
  // abort();
  exit(17);
}

void parent_process(int childpid)
{
  int wait_rv = 1;
  int child_status;
  int high_8, low_7, bit_7;

  wait_rv = wait(&child_status);
  printf("done waiting for %d. Wait returned %d\n", childpid, wait_rv);
  pr_exit(child_status);
  // high_8 = child_status >> 8;
  // low_7 = child_status && 0x7F;
  // bit_7 = child_status & 0x80;

  // printf("status: exit = %d, sig = %d, core = %d\n", high_8, low_7, bit_7);
}

void pr_exit(int status)
{
  if (WIFEXITED(status))
  {
    printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("abnormal termination, signal number = %d\n", WTERMSIG(status));
  }
  else if (WIFSTOPPED(status))
  {
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
  }
}