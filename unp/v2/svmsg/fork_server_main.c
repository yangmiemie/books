#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

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

void server(int, int);

void sig_child(int signo)
{
  int stat;
  int pid;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0);
  return;
}

int main(int argc, char const *argv[])
{
  int msqid;

  if ((msqid = msgget(ftok("./msgcreate.c", 0), SVMSG_MODE | IPC_CREAT)) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  signal(SIGCHLD, sig_child);

  printf("Server id: %d\n", msqid);
  server(msqid, msqid);

  return 0;
}

void server(int readid, int writeid)
{
  struct msgbuf buf;
  char* ptr;
  int n, fd;

  while (1)
  {
    buf.mtype = 1;
    if ((n = msgrcv(readid, &buf, MAX_MESG_SIZE, buf.mtype, 0)) < 0)
    {
      perror("msgrcv error");
      exit(1);
    }

    buf.mtext[n] = '\0';
    printf("Server received: %s\n", buf.mtext);

    if ((ptr = strchr(buf.mtext, ' ')) == NULL)
    {
      fprintf(stderr, "bogus request: %s\n", buf.mtext);
      continue;
    }

    *ptr++ = '\0';
    writeid = atoi(buf.mtext);

    if (fork() == 0)
    {
      if ((fd = open(ptr, O_RDONLY)) < 0)
      {
        sprintf(buf.mtext, "%s cannot open: %s", ptr, strerror(errno));
        buf.mtext[strlen(buf.mtext)] = '\0';
        printf("%s\n", buf.mtext);
        buf.mtype = 1;
        msgsnd(writeid, &buf, strlen(buf.mtext), 0);
      }
      else
      {
        printf("Server open file succeed and fd = %d\n", fd);
        while ((n = read(fd, buf.mtext, TEXT_SIZE)) > 0)
        {
          buf.mtype = 1;
          msgsnd(writeid, &buf, n, 0);
        }

        close(fd);
      }

      buf.mtype = 1;
      msgsnd(writeid, &buf, 0, 0);
      exit(1);
    }
  }
}