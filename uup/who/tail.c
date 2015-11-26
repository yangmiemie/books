#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFSIZE 64

struct tailbuf {
  char buf[BUFSIZE + 1];
  struct tailbuf* next;
};
typedef struct tailbuf TBUFFER;

int main(int argc, char const *argv[])
{
  tail(argv[1], atoi(argv[2]));
  return 0;
}

void err_ret(char* s1, char *s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int tail(char* pathname, int nlines)
{
  int fd, length, readlen, total_lines, read_lines;
  int i;
  int offset, first_offset;

  TBUFFER *first, *last;
  first = (TBUFFER* )malloc(sizeof (TBUFFER));
  TBUFFER *tmp, *next;
  first->next = NULL;

  if ((fd = open(pathname, O_RDONLY)) < 0)
  {
    err_ret("Cannot open", pathname);
  }

  length = lseek(fd, 0, SEEK_END);

  tmp = first;
  total_lines = 0;

  while(1)
  {
    if ((offset = lseek(fd, -1 * BUFSIZE, SEEK_CUR)) < 0)
    {
      err_ret("lseek error", "");
    }

    if ((readlen = read(fd, tmp->buf, BUFSIZE)) < 0)
    {
      err_ret("read error", "");
    }

    tmp->buf[readlen] = '\0';

    read_lines = lines_number(tmp->buf, readlen);
    total_lines += read_lines;

    first = tmp;

    // a line is divided into two part, one in last part of buf, one in begining of buf.
    // we need to find out the last part of buf.
    if (total_lines >= nlines)
    {
      break;
    }

    if ((offset = lseek(fd, -1 * readlen, SEEK_CUR)) < 0)
    {
      err_ret("lseek error", "");
    }

    tmp = (TBUFFER* )malloc(sizeof(TBUFFER));
    tmp->next = first;    
  }

  // first buf needs to read last several lines, and store its offset
  first_offset = lines_start(first, total_lines, nlines);

  for (i = first_offset; i < BUFSIZE; ++i)
  {
    putchar(first->buf[i]);
  }

  for (tmp = first->next; tmp; tmp = tmp->next)
  {
    for (i = 0; i < BUFSIZE ; ++i)
    {
      putchar(tmp->buf[i]);
    }
  }

  for (tmp = first; tmp; )
  {
    next = tmp->next;
    free(tmp);
    tmp = next;
  }
}

int lines_start(TBUFFER* pbuf, int total_lines, int print_lines)
{
  // taking the last line into account, for the last line end up with eof rather than '\n'
  int cur_lines = total_lines - print_lines + 1;
  int i = 0, lines = 0;

  for (i = 0; i < BUFSIZE; ++i)
  {
    if (pbuf->buf[i] == '\n')
    {
      ++lines;
      if (lines == cur_lines)
      {
        break;
      }
    }
  }

  return i + 1;
}

int lines_number(char* s, int size)
{
  int i, lines;

  lines = 0;
  for (i = 0; i < size; ++i)
  {
    if (s[i] == '\n' || s[i] == EOF)
    {
      ++lines;
    }
  }

  return lines;
}