#include <stdio.h>
#include <fcntl.h>
// #include <unistd.h>

#define BUFSIZE 32

void err_ret(char* s1, char *s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int main(int argc, char *argv[])
{
  char buf[BUFSIZE] = {0};
  int fd, offset, readlen;

  if ((fd = open(argv[1], O_RDONLY)) < 0)
  {
    err_ret("Cannot open", argv[1]);
  }

  offset = lseek(fd, 0, SEEK_END);

  if ((offset = lseek(fd, -1 * BUFSIZE, SEEK_CUR)) < 0)
  {
    err_ret("lseek error", "");
  }

  if ((readlen = read(fd, buf, BUFSIZE)) < 0)
  {
    err_ret("read error", "");
  }

  return 0;
}