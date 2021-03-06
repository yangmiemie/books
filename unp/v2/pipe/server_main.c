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
  int writefd, readfd;

  mkfifo(FIFO1, FIFO_MODE);
  mkfifo(FIFO2, FIFO_MODE);

  readfd = open(FIFO1, O_RDONLY);
  writefd = open(FIFO2, O_WRONLY);

  server(readfd, writefd);
  return 0;
}