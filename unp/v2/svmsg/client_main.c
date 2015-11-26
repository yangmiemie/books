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

void client(int readfd, int writefd);

int main(int argc, char const *argv[])
{
  int msqid;

  if ((msqid = msgget(ftok("./msgcreate.c", 0), SVMSG_MODE)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  printf("Client id: %d\n", msqid);
  client(msqid, msqid);

  return 0;
}


void client(int readfd, int writefd)
{
  struct msgbuf buf;
  int len, n, pid;

  pid = getpid();

  while (1)
  {
    memset(buf.mtext, 0, TEXT_SIZE);
    sprintf(buf.mtext, "%d ", pid);
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

    if (msgsnd(writefd, &buf, len, 0) < 0)
    {
      perror("msgsnd error");
      exit(1);
    }

    while ((n = msgrcv(readfd, &buf, MAX_MESG_SIZE, pid, 0)) > 0)
    {
      write(1, buf.mtext, n);
      // perror("msgrcv error");
      // exit(1);
    }
    printf("\n");
  }
}