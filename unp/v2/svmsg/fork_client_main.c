#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

extern int errno;

#define SVMSG_MODE 0666
#define TEXT_SIZE 512
#define MAX_MESG_SIZE (TEXT_SIZE + 4)
struct msgbuf
{
  long mtype ;
  // int length;
  char mtext[TEXT_SIZE] ;
};

void client(int readid, int writeid);

int main(int argc, char const *argv[])
{
  int readid, writeid;

  if ((writeid = msgget(ftok("./msgcreate.c", 0), SVMSG_MODE)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  if ((readid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  printf("Client readid: %d, writeid: %d\n", readid, writeid);
  client(readid, writeid);

  return 0;
}


void client(int readid, int writeid)
{
  struct msgbuf buf;
  int len, n;

  while (1)
  {
    memset(buf.mtext, 0, TEXT_SIZE);
    sprintf(buf.mtext, "%d ", readid);
    len = strlen(buf.mtext);

    read(0, buf.mtext + len, TEXT_SIZE - len);
    len = strlen(buf.mtext);

    if (buf.mtext[len-1] == '\n')
    {
      printf("Last c is newline\n");
      buf.mtext[len-1] = '\0';
      len--;
    } 

    printf("Client message: %s\n", buf.mtext);
    printf("Client message length: %d\n", len);
    buf.mtype = 1;

    if (msgsnd(writeid, &buf, len, 0) < 0)
    {
      perror("msgsnd error");
      exit(1);
    }

    while ((n = msgrcv(readid, &buf, MAX_MESG_SIZE, 1, 0)) > 0)
    {
      write(1, buf.mtext, n);
      // perror("msgrcv error");
      // exit(1);
    }
    printf("\n");
  }
}