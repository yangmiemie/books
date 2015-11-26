#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;

#define LEN 256

void client(int readfd, int writefd)
{
  int n;
  int status;
  char buffer[LEN] = {0};

  while ((n = read(0, buffer, LEN)) > 0)
  { 
    buffer[n-1] = '\0';
    printf("Client write %d bytes\n", n - 1);
    write(writefd, buffer, n - 1);

    while ((n = read(readfd, buffer, LEN)) > 0)
    {
      buffer[n] = '\0';
      printf("%s", buffer);
    }
  }

  printf("Read from STDIN is %d\n", n);
  printf("End of input\n");
  close(writefd);
  waitpid(-1, &status, 0);
}

void server(int readfd, int writefd)
{
  int fd, n;
  char content[LEN] = {0};
  char buffer[LEN] = {0};

  while ((n = read(readfd, buffer, LEN)) > 0)
  {
    // printf("Server read %d bytes\n", n);
    // write(writefd, buffer, n);
    buffer[n] = '\0';

    if ((fd = open(buffer, O_RDONLY)) == -1)
    {
      sprintf(content, "%s open error for %d\n", buffer, errno);
      perror(content);
      write(writefd, content, strlen(content));
    }
    else
    {
      while ((n = read(fd, content, LEN)) > 0)
      {
        printf("Read %d bytes from %s\n", n, buffer);
        write(writefd, content, n);
      }
      printf("At last n is %d\n", n);
      close(fd);
    }
  }
  sleep(2);
  printf("End of reading for n = %d\n", n);   
}
