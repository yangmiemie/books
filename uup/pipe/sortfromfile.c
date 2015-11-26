#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void err_ret(char* s1, char *s2);

int main(int argc, char *argv[])
{
  int fd, pid;

  if ((pid = fork()) == -1)
    err_ret("fork", "");
  else if (pid == 0)
  {
    fd = open(argv[1], O_RDONLY);
    close(0);
    dup(fd);
    close(fd);

    execlp("sort", "sort", NULL);
  }
  else
  {
    wait(NULL);
  }
  return 0;
}

void err_ret(char* s1, char *s2)
{
  fprintf(stderr, "Error: %s\n", s1);
  perror(s2);
  exit(1);
}