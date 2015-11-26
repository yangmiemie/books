#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "comm.h"

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define FIFO_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int main(int argc, char const *argv[])
{
  int pid;
  int writefd, readfd;

  mkfifo(FIFO1, FIFO_MODE);
  mkfifo(FIFO2, FIFO_MODE);

  if ((pid = fork()) == -1)
  {
    fprintf(stderr, "Fork error");
    exit(1);
  }
  else if (pid > 0)
  {
    writefd = open(FIFO1, O_WRONLY, 0);
    readfd = open(FIFO2, O_RDONLY, 0);
    client(readfd, writefd);
  }
  else
  {
    writefd = open(FIFO2, O_WRONLY, 0);
    readfd = open(FIFO1, O_RDONLY, 0);
    server(readfd, writefd);
  }

  unlink(FIFO1);
  unlink(FIFO2);

  return 0;
}