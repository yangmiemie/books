#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 512

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}
int main(int argc, char *argv[])
{
  int fd;
  char buf[BUFSIZE];

  if (argc != 2)
  { 
    err_ret("usage: write", "");
  } 

  if ((fd = open(argv[1], O_WRONLY)) == -1)
  {
    err_ret("Cannot open file", argv[1]);
  }

  while((fgets(buf, BUFSIZE, stdin)) != NULL)
  {
    write(fd, buf, strlen(buf));
  }

  return 0;
}