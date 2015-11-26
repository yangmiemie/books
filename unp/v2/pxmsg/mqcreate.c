#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

extern int optind;
#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int main(int argc, char *argv[])
{
  mqd_t mqd;
  int c, flags;
  struct mq_attr attr;

  flags = O_RDWR | O_CREAT;
  memset(&attr, 0, sizeof(attr));

  while ((c = getopt(argc, argv, "em:z:")) != -1)
  {
    switch(c)
    {
      case 'e': 
        flags |= O_EXCL;
        break;
      case 'm':
        attr.mq_maxmsg = atoi(optarg);
        break;
      case 'z':
        attr.mq_msgsize = atoi(optarg);
        break;
    }
  }

  printf("optind: %d\n", optind);
  printf("argc: %d\n", argc);
  printf("mq_maxmsg: %d\n", attr.mq_maxmsg);
  printf("mq_msgsize: %d\n", attr.mq_msgsize);

  if (optind != argc - 1)
  {
    fprintf(stderr, "usage: mqcreate [-e] <name>\n");
    exit(1);
  }

  if ((attr.mq_msgsize == 0 && attr.mq_maxmsg != 0) ||
      (attr.mq_msgsize != 0 && attr.mq_maxmsg == 0))
  {
    fprintf(stderr, "must specify both -m mq_maxmsg and -z mq_msgsize\n");
    exit(1);
  }

  printf("filename: %s\n", argv[optind]);
  if ((mqd = mq_open(argv[optind], flags, FILE_MODE, (attr.mq_msgsize != 0) ? &attr : NULL)) == -1)
  {
    perror("Create mq error: ");
    exit(1);
  }

  mq_close(mqd);
  return 0;
}