#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void notify_thread(union sigval);

mqd_t mqd;
char *buffer;
int len;
struct sigevent sigev;

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: mqnotifysig1 <name>");
    exit(1);
  }

  struct mq_attr attr;

  if ((mqd = mq_open(argv[1], O_RDONLY)) == -1)
  {
    perror("mq open error");
    exit(1);
  }

  mq_getattr(mqd, &attr);
  len = attr.mq_msgsize;
  buffer = malloc(len);
  memset(buffer, 0, len);

  sigev.sigev_notify = SIGEV_THREAD;
  sigev.sigev_value.sival_ptr = NULL;
  sigev.sigev_notify_function = notify_thread;
  sigev.sigev_notify_attributes = NULL;

  if (mq_notify(mqd, &sigev) == -1)
  {
    perror("mq notify error");
    exit(1);
  }

  while (1)
    pause();

  return 0;
}

void notify_thread(union sigval arg)
{
  int n, prio;

  if (mq_notify(mqd, &sigev) == -1)
  {
    perror("mq notify error");
    exit(1);
  }
  if ((n = mq_receive(mqd, buffer, len, &prio)) == -1)
  {
    perror("receive mq error");
    exit(1);
  }

  printf("SIGUSR1 received, read %d bytes, buffer: %s\n", n, buffer);
}