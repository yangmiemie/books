#include <stdio.h>
#include <fcntl.h>

void err_ret(char *s1, char *s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

#define MODE 0644
#define BUFSIZE 1024

int get_line(char* buf, int size);
int exist(char* pathname);

int main(int argc, char *argv[])
{
  int in_fd, out_fd, readlen, c;
  int interactive_flag = 0;
  char *ptr;
  char buf[BUFSIZE];

  // if (argc !=  3 || argc != 4)
  // {
  //  fprintf(stderr, "usage: %s source destination\n", *argv);
  //  exit(1);
  // }

  while(--argc > 0 && **++argv == '-')
  {
    while((c = *++*argv) != '\0')
    {
      switch(c)
      {
        case 'i':
          interactive_flag = 1;
          break;
        default:
          break;
      }
    }
  }

  if (argc < 2)
  {
    fprintf(stderr, "missing file for cp\n");
    exit(1);
  }

  if (strcmp(*argv, *(argv + 1)) == 0)
  {
    fprintf(stderr, "cp: '%s' and '%s' are the same file\n", argv[1], argv[2]);
    exit(1);
  }

  if ((out_fd = open(*argv, O_RDONLY)) < 0)
  {
    err_ret("Can't open", argv[1]);
  }

  if (exist(*(argv + 1)) && interactive_flag == 1)
  {
    printf("cp: overwrite '%s'? ", *(argv + 1));
    if (get_line(buf, BUFSIZE) > 0 && buf[0] != 'y')
    {
      return;
    }
  }

  if ((in_fd = creat(*(argv + 1), MODE)) < 0)
  {
    err_ret("Can't create", argv[2]);
  }

  while((readlen = read(out_fd, buf, BUFSIZE)) > 0)
  {
    write(in_fd, buf, readlen);
  }

  if (close(in_fd) != 0 || close(out_fd) != 0 )
  {
    err_ret("Close file failed", "");
  }

  return 0;
}

int exist(char* pathname)
{
  int fd;

  fd = open(pathname, O_RDONLY);

  return fd != -1;
}

int get_line(char* buf, int size)
{
  int c, i;

  i = 0;
  while(((c = getchar()) != EOF) && c != '\n' && i < size)
  {
    buf[i++] = c;
  }

  if (c == '\n')
  {
    buf[i++] = c;
  }

  buf[i] = '\0';

  return i;
}