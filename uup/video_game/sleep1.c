#include <stdio.h>
#include <signal.h>
#define SHHHH

void wakeup(int signum);

int main(int argc, char const *argv[])
{
  printf("about to sleep for 4 seconds\n");
  signal(SIGALRM, wakeup);
  alarm(4);
  pause();
  printf("Morning so soon?\n");
  return 0;
}

void wakeup(int signum)
{
#ifndef SHHHH
  printf("Alarm received from kernel\n");
#endif
}