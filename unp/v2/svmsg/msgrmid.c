#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SVMSG_MODE 0666

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: msgrmid <pathname>");
    exit(1);
  }

  int mqid;

  if((mqid = msgget(ftok(argv[1], 0), SVMSG_MODE)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  if (msgctl(mqid, IPC_RMID, NULL) < 0)
  {
    perror("msgctl error");
    exit(1);
  }

  return 0;
}