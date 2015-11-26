#include <mqueue.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void sigusr1(int signo);

int pipefd[2];

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: mqnotifysig1 <name>");
    exit(1);
  }

  struct sigevent sigev;
  struct mq_attr attr;
  int n, prio;
  int signo;
  mqd_t mqd;
  char *buffer;
  int len;
  fd_set set;
  int nfds;

  sigset_t newmask;

  if ((mqd = mq_open(argv[1], O_RDONLY)) == -1)
  {
    perror("open mq error");
    exit(1);
  }

  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = SIGUSR1;

  mq_getattr(mqd, &attr);
  len = attr.mq_msgsize;
  buffer = malloc(len);
  memset(buffer, 0, len);

  pipe(pipefd);

  signal(SIGUSR1, sigusr1);

  // sigemptyset(&newmask);
  // sigaddset(&newmask, SIGUSR1);

  if ((mq_notify(mqd, &sigev)) == -1)
  {
    perror("notify mq error");
    exit(1);
  }

  FD_ZERO(&set);
  FD_SET(pipefd[0], &set);

  while (1)
  {
    nfds = select(pipefd[1] + 1, &set, NULL, NULL, NULL);
    if (FD_ISSET(pipefd[0], &set))
    {
      if ((n = mq_receive(mqd, buffer, len, &prio)) == -1)
      {
        perror("receive mq error");
        exit(1);
      }
      printf("SIGUSR1 received, read %d bytes, buffer: %s\n", n, buffer);     
    }
  }

  return 0;
}

void sigusr1(int signo)
{
  write(pipefd[1], "", 1);
  return;
}