#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define DELAY 5

void child_process(int delay);
void parent_process(int childpid);
void pr_exit(int status);
void child_handler(int signum);
void int_handler(int signum);

int done = 0;

int main(int argc, char const *argv[])
{
  int newpid;
  int i;
  int child_status, child_pid;
  int number;
  struct sigaction newhandler;

  number = atoi(argv[1]);

  printf("Before: pid is %d\n", getpid());

  // for (i = 0; i < 3; ++i)
  // {
  //  newpid = fork();
  //  if (newpid == 0)
  //    child_process(1);

  //  parent_process(newpid);
  // }

  newhandler.sa_handler = child_handler;
  newhandler.sa_flags = SA_NODEFER;
  if (sigaction(SIGCHLD, &newhandler, NULL) == -1)
  {
    perror("sigaction");
  }


  // signal(SIGCHLD, child_handler);

  for (i = 0; i < number; ++i)
  {
    newpid = fork();
    if (newpid == 0)
    {
      child_process(5 + i);
    }
  }

  while(done != number)
  {
    printf("waiting...\n");
    sleep(1);
  }

  return 0;
}


void int_handler(int signum)
{
  printf("Called with signal %d\n", signum);
  sleep(5);
  ++done;
  printf("Done handling signal %d\n", signum);
}

void child_handler(int signum)
{
  int child_status, child_pid;
  printf("This is the SIGCHLD handler\n");

  child_pid = wait(&child_status);
  printf("exit child: %d, ", child_pid);
  pr_exit(child_status);

  // sleep(3);

  printf("Done in child_handler\n");

  ++done;
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