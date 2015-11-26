#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 64

int get_mode(char* pathname);

void err_ret(char* s1, char *s2)
{
  fprintf(stderr, "Error: %s", s1);
  perror(s2);
  exit(1);
}

int main(int argc, char *argv[])
{
  int mode1, mode2;
  char buf[BUFSIZE];

  if (argc != 3)
  {
    err_ret("the number of input argument should be 3 ", "");
  }

  mode1 = get_mode(argv[1]);

  if (mode1 == -1)
  {
    err_ret("Cannot find ", argv[1]);
  }

  if (!S_ISREG(mode1))
  {
    err_ret("the first input should be a file ", argv[1]);
  }

  mode2 = get_mode(argv[2]);

  if (mode2 == -1 || S_ISREG(mode2))
  {
    rename(argv[1], argv[2]);
  }
  else if (S_ISDIR(mode2))
  {
    sprintf(buf, "%s/%s", argv[2], argv[1]);

    if (link(argv[1], buf) != -1)
    {
      unlink(argv[1]);
    }
    else
    {
      perror(buf);
    }
  }

  return 0;
}

// if the file is not existed, -1 is returned
int get_mode(char* pathname)
{
  struct stat sbuf;
  if (stat(pathname, &sbuf) == -1)
  {
    return -1;
  }

  return sbuf.st_mode;
}