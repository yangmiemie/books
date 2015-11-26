#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int flags, c, n, prio;
  mqd_t mqd;
  char *buffer;
  struct mq_attr attr;

  flags = O_RDONLY;

  while ((c = getopt(argc, argv, "n")) != -1)
  {
    switch(c)
    {
      case 'n':
        flags |= O_NONBLOCK;
      break;
    }
  }

  if (optind != argc - 1)
  {
    fprintf(stderr, "usage: mqreceive [ -n ] <name>\n");
    exit(1);
  }

  if ((mqd = mq_open(argv[argc-1], flags)) == -1)
  {
    perror("mq open error");
    exit(1);
  }

  mq_getattr(mqd, &attr);

  buffer = malloc(attr.mq_msgsize);

  if ((n = mq_receive(mqd, buffer, attr.mq_msgsize, &prio)) == -1)
  {
    perror("mq receive error");
    exit(1);
  }

  printf("read %d bytes, priority = %d, buffer = %s\n", n, prio, buffer);

  return 0;
}

