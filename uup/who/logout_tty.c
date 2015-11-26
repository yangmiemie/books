#include <utmp.h>
#include <fcntl.h>
#include <stdio.h>

int logout_tty(char* line);

// void err_ret(char* s1, char* s2)
// {
//  fprintf(stderr, "Error: %s ", s1);
//  perror(s2);
//  exit(1);
// }

int main(int argc, char const *argv[])
{
  if (logout_tty(argv[1]) < 0)
  {
    fprintf(stderr, "logout_tty error\n");
    exit(1);
  }
  return 0;
}

int logout_tty(char* line)
{
  struct utmp *ptr;
  int reval = -1;

  utmp_open(UTMP_FILE);

  while((ptr = utmp_next()) != NULL)
  {
    if (ptr->ut_type != USER_PROCESS)
    {
      continue;
    }

    if (strcmp(ptr->ut_line, line) == 0)
    {
      ptr->ut_type = USER_PROCESS;
      if (utmp_seek(-1, SEEK_CUR) != -1)
        if (utmp_write(ptr))
        {
          reval = 1;
        }
    }
  }

  if (utmp_close() != 0)
  {
    reval = -1;
  }

  return reval;
}

// int logout_tty(char* line)
// {
//  int fd, readlen;
//  struct utmp utbuf;
//  int utlen = sizeof(utbuf);
//  int reval = -1;

//  if ((fd = open(UTMP_FILE, O_RDWR)) < 0)
//  {
//    err_ret("Cannot open", UTMP_FILE);
//  }

//  while((readlen = read(fd, &utbuf, utlen)) > 0)
//  {
//    if (utbuf.ut_type != USER_PROCESS)
//    {
//      continue;
//    }

//    if (strcmp(utbuf.ut_line, line) == 0)
//    {
//      utbuf.ut_type = DEAD_PROCESS;
//      if (time(&utbuf.ut_time) != -1)
//      {
//        if (lseek(fd, -readlen, SEEK_CUR) != -1)
//          if (write(fd, &utbuf, utlen) == utlen)
//            reval = 0;
//      }
//    }
//  }

//  if (close(fd) == -1)
//  {
//    reval = -1;
//  }

//  return reval;
// }