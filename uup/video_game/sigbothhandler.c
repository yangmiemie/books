#include <signal.h>

void both_handler(int signum);

int main(int argc, char const *argv[])
{
  signal(SIGQUIT, both_handler);
  signal(SIGINT, both_handler);

  while(1)
    pause();

  return 0;
}

void both_handler(int signum)
{
  if (signum == SIGINT)
  {
    printf("This is SIGINT handler\n");
    sleep(2);
  }
  else if (signum == SIGQUIT)
  {
    printf("This is SIGQUIT handler\n");
    sleep(2);
  }
}