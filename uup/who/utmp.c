#include <utmp.h>
#include <stdio.h>
#include <time.h>

void show_info(struct utmp *utbufp);

int main(int argc, char const *argv[])
{
  struct utmp *putmp;

  utmp_open(UTMP_FILE);

  while((putmp = utmp_next()) != (struct utmp *)NULL)
  {
    show_info(putmp);
  }

  utmp_close();
  return 0;
}

void show_info(struct utmp *utbufp)
{
  // if (utbufp->ut_type != USER_PROCESS)
  // {
  //  return;
  // }

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