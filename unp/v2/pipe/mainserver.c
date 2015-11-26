#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_SERVER "/tmp/fifo.server"
#define FIFO_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define MAX 256

int main(int argc, char const *argv[])
{
  int writefd, readfd, dummyfd;
  char buffer[MAX] = {0};
  char filename[MAX] = {0};
  char content[MAX] = {0};
  char *ptr;
  int childpid;
  int fd;
  int n;

  mkfifo(FIFO_SERVER, FIFO_MODE);

  readfd = open(FIFO_SERVER, O_RDONLY);
  dummyfd = open(FIFO_SERVER, O_WRONLY);

  while ((n = read(readfd, buffer, MAX)) > 0)
  {
    if (n == 0)
    {
      printf("Client %d has closed\n", readfd);
      continue;
    }

    buffer[n] = '\0';
    if ((ptr = strchr(buffer, ' ')) == NULL)
    {
      fprintf(stderr, "Error input from client: %s\n", buffer);
      continue;
    }

    *ptr++ = '\0';

    printf("filename read from input: %s\n", buffer);

    childpid = atoi(ptr);
    sprintf(filename, "/tmp/fifo.%d", childpid);

    printf("pid read from input: %s\n", filename);

    mkfifo(filename, FIFO_MODE);
    writefd = open(filename, O_WRONLY);

    printf("writefd is %d\n", writefd);

    if ((fd = open(buffer, O_RDONLY)) == -1)
    {
      sprintf(content, "Open file %s error\n", buffer);
      write(writefd, content, strlen(content));
    }
    else
    {
      while ((n = read(fd, content, MAX)) > 0)
      {
        write(writefd, content, n);
      }
      close(fd);
    }
    
    close(writefd);
  }

  printf("Exit for n = %d\n", n);
  return 0;
}