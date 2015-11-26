#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  int pid;
  int fd;

  if ((pid = fork()) == 0)
  {
    close(1);
    fd = open("userlist", O_CREAT | O_WRONLY, 0664);
    lseek(fd, 0, SEEK_END);
    execlp("who", "who", NULL);
  }
  else
  {
    wait(NULL);
    printf("Done running who\n");
  }

  return 0;
}