#include <mqueue.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

void sigusr1(int signo);
mqd_t mqd;
char *buffer;
int len;

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: mqnotifysig1 <name>");
    exit(1);
  }

  struct sigevent sigev;
  struct mq_attr attr;

  if ((mqd = mq_open(argv[1], O_RDONLY)) == -1)
  {
    perror("open mq error");
    exit(1);
  }

  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = SIGUSR1;

  signal(SIGUSR1, sigusr1);

  mq_getattr(mqd, &attr);
  len = attr.mq_msgsize;
  buffer = malloc(len);
  memset(buffer, 0, len);

  if ((mq_notify(mqd, &sigev)) == -1)
  {
    perror("notify mq error");
    exit(1);
  }

  while (1)
    pause();

  return 0;
}

void sigusr1(int signo)
{
  int n, prio;

  if ((n = mq_receive(mqd, buffer, len, &prio)) == -1)
  {
    perror("receive mq error");
    exit(1);
  }

  printf("SIGUSR1 received, read %d bytes, buffer: %s\n", n, buffer);
}