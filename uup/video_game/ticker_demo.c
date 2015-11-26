#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

void count_down(int signum);
void set_ticker(int msecs);

int done = 0;

int main(int argc, char const *argv[])
{
  signal(SIGALRM, count_down);

  set_ticker(200);

  while(!done)
    pause();

  return 0;
}

void count_down(int signum)
{
  static int num = 10;
  printf("%d...", num--);
  fflush(stdout);

  if (num < 0)
  {
    printf("DONE!\n");
    done = 1;
  }
}

void set_ticker(int msecs)
{
  struct itimerval new_timerset;
  int secs = msecs / 1000;
  int usecs = (msecs % 1000) * 1000;

  new_timerset.it_interval.tv_sec = secs;
  new_timerset.it_interval.tv_usec = usecs;
  
  new_timerset.it_value.tv_sec = secs;
  new_timerset.it_value.tv_usec = usecs;

  // setitimer(ITIMER_VIRTUAL, &new_timerset, NULL);

  setitimer(ITIMER_REAL, &new_timerset, NULL);
}