#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAX 256
#define FIFO_SERVER "/tmp/fifo.server"
#define FIFO_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int main(int argc, char const *argv[])
{
  int childpid;
  char filename[MAX] = {0};
  char content[MAX] = {0};
  int n;
  int writefd, readfd;

  childpid = getpid();

  if ((n = read(0, filename, MAX)) > 0)
  {
    if (filename[n-1] == '\n')
    {
      filename[n-1] = '\0';
    }

    printf("filename is %s\n", filename);

    sprintf(content, "%s %d", filename, childpid);

    sprintf(filename, "/tmp/fifo.%d", childpid);
    printf("filename is %s\n", filename);

    mkfifo(filename, FIFO_MODE);

    writefd = open(FIFO_SERVER, O_WRONLY);

    printf("write content is %s\n", content);

    write(writefd, content, strlen(content));

    readfd = open(filename, O_RDONLY, 0);
    while ((n = read(readfd, content, MAX)) > 0)
      write(1, content, n);

    close(readfd);
    close(writefd);

    unlink(filename);
  }

  exit(0);
  return 0;
}