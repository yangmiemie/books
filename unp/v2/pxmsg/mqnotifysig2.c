#include <mqueue.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

void sigusr1(int signo);
mqd_t mqd;
char *buffer;
int len;
int mqflag;

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

  sigset_t zeromask, newmask, oldmask;

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
  mqflag = 0;

  sigemptyset(&zeromask);
  sigemptyset(&newmask);
  sigemptyset(&oldmask);

  sigaddset(&newmask, SIGUSR1);

  if ((mq_notify(mqd, &sigev)) == -1)
  {
    perror("notify mq error");
    exit(1);
  }

  while (1)
  {
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    printf("before pause, mqflag = %d\n", mqflag);
    while (mqflag == 0)
      sigsuspend(&zeromask);

    mqflag = 0;

    printf("before mq_receive, mqflag = %d\n", mqflag);

    if ((mq_notify(mqd, &sigev)) == -1)
    {
      perror("notify mq error");
      exit(1);
    }

    if ((n = mq_receive(mqd, buffer, len, &prio)) == -1)
    {
      perror("receive mq error");
      exit(1);
    }

    printf("SIGUSR1 received, read %d bytes, buffer: %s\n", n, buffer);

    sigprocmask(SIG_UNBLOCK, &newmask, NULL);
  }

  return 0;
}

void sigusr1(int signo)
{
  mqflag = 1;
  return;
}