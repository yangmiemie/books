#include <signal.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  int pid;

  if (argc != 2)
  {
    fprintf(stderr, "usage: sender <pid>\n");
    exit(1);
  }

  pid = atoi(argv[1]);

  kill(pid, SIGCHLD);
  // sleep(1);

  kill(pid, SIGCHLD);
  // sleep(1);

  kill(pid, SIGCHLD);
  // sleep(1);

  kill(pid, SIGCHLD);
  // sleep(1);

  kill(pid, SIGCHLD);
  // sleep(1);

  kill(pid, SIGCHLD);
  // sleep(1);

  kill(pid, SIGCHLD);
  // sleep(1);
  return 0;
}