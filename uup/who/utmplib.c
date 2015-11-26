#include <stdio.h>
#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>

#define UTMP_SIZE (sizeof(struct utmp))
#define UTMP_NUM 16

static int cur_record;
static int num_record;
static char utmpbuf[UTMP_NUM * UTMP_SIZE];
static int utfd = -1;

void err_ret(char* s1, char *s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int utmp_open(char* filename)
{
  if ((utfd = open(filename, O_RDONLY)) < 0)
  {
    err_ret("Cannot open", filename);
  }

  cur_record = num_record = 0;

  return utfd;
}

struct utmp* utmp_next()
{
  struct utmp *putmp;

  while(1)
  {
    if (cur_record == num_record && utmp_reload() == 0)
    {
      return NULL;
    }

    putmp = (struct utmp*)(utmpbuf + UTMP_SIZE * cur_record);
    cur_record++;

    if (putmp->ut_type == USER_PROCESS)
    {
      return putmp;
    }
  }
}

int utmp_reload()
{
  int len;

  if (((len  = read(utfd, utmpbuf, UTMP_NUM * UTMP_SIZE))) <= 0)
  {
    return 0;
  }

  num_record = len / UTMP_SIZE;
  cur_record = 0;

  return num_record;
}

int utmp_close()
{
  if (utfd != -1)
  {
    return close(utfd);
  }

  return 0;
}

int utmp_seek(int offset, int whence)
{
  int off;

  if (whence == SEEK_CUR)
  {
    offset += (cur_record - num_record);
  }

  off = lseek(utfd, offset, whence);

  if (off == -1)
  {
    return -1;
  }

  num_record = cur_record;

  return off / UTMP_SIZE;
}

int utmp_write(struct utmp *ptr)
{
  if (write(utfd, ptr, UTMP_SIZE) != UTMP_SIZE)
  {
    return -1;
  }

  return 1;
}