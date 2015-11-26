#include <signal.h>
#include <stdio.h>

void int_handler(int signum);

int cur_times = 0;
int times = 0;
char *msg;

int main(int argc, char *argv[])
{
  msg = argv[1];
  times = atoi(argv[2]);

  signal(SIGINT, int_handler);

  while(1)
    pause();

  return 0;
}

void int_handler(int signum)
{
  if (times == cur_times)
  {
    cur_times = 0;
    printf("%s\n", msg);
  }
  else
  {
    ++cur_times;
  }
}