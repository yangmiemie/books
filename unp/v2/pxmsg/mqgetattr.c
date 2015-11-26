#include <mqueue.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  struct mq_attr attr;
  mqd_t mqd;

  if (argc != 2)
  {
    fprintf(stderr, "usage: mqgetattr <name>");
    exit(1);
  }

  mqd = mq_open(argv[1], O_RDONLY);

  mq_getattr(mqd, &attr);

  printf("max messages: %d, max message size: %d, current messages: %d\n", 
    attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

  return 0;
}