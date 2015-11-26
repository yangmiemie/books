#include <stdio.h>
#include <utmp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

void show_info(struct utmp *utbufp);
void showtime(long timeval);

int main(int argc, char const *argv[])
{
  struct utmp current_record;
  int fd;
  int readlen = sizeof(current_record);

  if ((fd = open(UTMP_FILE, O_RDONLY)) <= 0)
  {
    fprintf(stderr, "open file %s error, errno = %d\n", UTMP_FILE, errno);
  }

  while((read(fd, &current_record, readlen)) > 0)
  {
    show_info(&current_record);
  }

  close(fd);

  return 0;
}

void show_info(struct utmp *utbufp)
{
  if (utbufp->ut_type != USER_PROCESS)
  {
    return;
  }

  printf("%-16s", utbufp->ut_user);
  printf("%-8s", utbufp->ut_line);
  printf("%-16d", (utbufp->ut_time));
  showtime(utbufp->ut_time);

  long curtime;
  char buffer[32];
  curtime = utbufp->ut_time;
  strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
  printf("%s", buffer);
  printf("\n");
}

void showtime(long timeval)
{
  char* cp;
  cp = ctime(&timeval);
  printf("%12.12s ", cp + 4);
}