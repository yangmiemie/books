#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>

#define SVMSG_MODE 0666
extern int optind;

int main(int argc, char *argv[])
{
  int c, flags, mqid;

  flags = SVMSG_MODE | IPC_CREAT;

  while ((c = getopt(argc, argv, "e")) != -1)
  {
    switch(c)
    {
      case 'e':
        flags |= IPC_EXCL;
      break;
    }
  }

  if (optind != argc - 1)
  {
    fprintf(stderr, "usage: mqcreate [ -e ] <pathname>%s\n");
    exit(1);
  }

  if ((mqid = msgget(ftok(argv[1], 0), flags)) < 0)
  {
    perror("msget error");
    exit(1);
  }

  return 0;
}