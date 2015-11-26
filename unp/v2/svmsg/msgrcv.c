#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#define SVMSG_MODE 0666
#define TEXT_SIZE 512

struct msgbuf
{
  long mtype ;
  char mtext[TEXT_SIZE] ;
};

extern char *optarg;
extern int optind;

int main(int argc, char *argv[])
{
  int mqid, c, flags, n;
  int type;
  struct msgbuf *buf;

  flags = 0;
  while ((c = getopt(argc, argv, "nt:")) != -1)
  {
    switch(c)
    {
      case 't':
        type = atoi(optarg);
        break;
      case 'n':
        flags |= IPC_NOWAIT;
        break;
    }
  }

  buf = malloc(sizeof(struct msgbuf));

  if (optind != argc - 1)
  {
    fprintf(stderr, "usage: msgrcv [ -t -n ] <pathname>\n");
    exit(1);
  }

  if ((mqid = msgget(ftok(argv[optind], 0), SVMSG_MODE)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  if ((n = msgrcv(mqid, buf, TEXT_SIZE, type, flags)) < 0)
  {
    perror("msgrcv error");
    exit(1);
  }

  printf("read %d bytes, type = %d, buf: %s\n", n, buf -> mtype, buf -> mtext);
  return 0;
}