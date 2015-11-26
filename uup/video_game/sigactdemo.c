#include <stdio.h>
#include <signal.h>
#define INPUTLEN 100

void int_handler(int signum);

int main(int argc, char const *argv[])
{
  struct sigaction newhandler;
  sigset_t blocked;
  char x[INPUTLEN];
  int nchars;

  newhandler.sa_handler = int_handler;
  // newhandler.sa_flags = SA_SIGINFO | SA_RESTART;
  // newhandler.sa_flags = SA_RESETHAND;

  sigemptyset(&blocked);
  sigaddset(&blocked, SIGQUIT);

  newhandler.sa_mask = blocked;

  // signal(SIGQUIT, int_handler);

  // sigemptyset(&blocked);
  // sigaddset(&blocked, SIGRTMIN);

  // newhandler.sa_mask = blocked;
  // sigaction(SIGRTMIN, &newhandler, NULL);

  if (sigaction(SIGINT, &newhandler, NULL) == -1)
  {
    perror("sigaction");
  }
  else
  {
    while(1)
    {
      nchars = read(0, x, INPUTLEN);
      if (nchars == -1)
      {
        perror("read return an error");
      }
      else
      {
        x[nchars] = '\0';
        printf("input: %s\n", x);       
      }
    }
  }
  return 0;
}

void int_handler(int signum)
{
  printf("Called with signal %d\n", signum);
  sleep(5);
  printf("Done handling signal %d\n", signum);
}