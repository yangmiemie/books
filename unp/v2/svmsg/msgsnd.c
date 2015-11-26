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

int main(int argc, char const *argv[])
{
  if (argc != 4)
  {
    fprintf(stderr, "usage: msgsnd <pathname> <#bytes> <type>\n");
    exit(1);
  }

  int mqid, len, type;
  struct msgbuf buf;

  len = atoi(argv[2]);
  type = atoi(argv[3]);

  if ((mqid = msgget(ftok(argv[1], 0), SVMSG_MODE)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  buf.mtype = type;
  memset(buf.mtext, 'a', len);

  if ((msgsnd(mqid, &buf, len, 0)) < 0)
  {
    perror("msgsnd error");
    exit(1);
  }

  return 0;
}