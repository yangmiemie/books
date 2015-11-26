#include <utmp.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>

#define UT_NUM 16
#define UT_SIZE (sizeof(struct utmp))

static char utbuf[UT_SIZE * UT_NUM];
static int cur_rec = 0;
static int num_rec = 0;
static int utfd = -1;

void show_info(struct utmp *utbufp);
void err_ret(char *s1, char *s2);
int utmp_open(char *filename);
int utmp_close();
struct utmp *utmp_next();
int utmp_reload();
void showtime(long timeval);
void show_info(struct utmp *utbufp);
char* whats_my_line(int fd);

int main(int argc, char const *argv[])
{
  struct utmp *putmp;
  char *ptty;

  utmp_open(UTMP_FILE);

  ptty = whats_my_line(1);

  while((putmp = utmp_next()) != NULL)
  {
    if ((ptty != NULL) && strcmp(ptty + 5, putmp->ut_line) == 0)
    {
      show_info(putmp);
    }
  }

  utmp_close();
  return 0;
}

char* whats_my_line(int fd)
{
  char* ptty;

  if ((ptty = ttyname(fd)) == NULL)
  {
    return NULL;
  }

  return ptty;
}

void err_ret(char *s1, char *s2)
{
  fprintf(stderr, "Error %s ", s1);
  perror(s2);
  exit(1);
}

int utmp_open(char *filename)
{
  if ((utfd = open(filename, O_RDONLY)) < 0)
  {
    err_ret("Cannot open", filename);
  }

  return 0;
}

int utmp_close()
{
  if (utfd > 0)
  {
    return close(utfd);
  }

  return -1;
}

struct utmp *utmp_next()
{
  struct utmp *putmp;

  if ((cur_rec == num_rec) && (utmp_reload() == 0))
  {
    return NULL;
  }

  putmp = (struct utmp*)(utbuf + cur_rec * UT_SIZE);
  ++cur_rec;

  return putmp;
}

int utmp_reload()
{
  int readlen;
  int utnum;

  if((readlen = read(utfd, utbuf, UT_SIZE * UT_NUM)) < 0)
  {
    return 0;
  }

  utnum = readlen / UT_SIZE;
  cur_rec = 0;
  num_rec = utnum;
  return utnum;
}

void showtime(long timeval)
{
  char* cp;
  cp = ctime(&timeval);
  printf("%12.12s ", cp + 4);
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
