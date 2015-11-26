#include <signal.h>
#include <stdio.h>

void f(int signum);

int main(int argc, char const *argv[])
{
  int i;

  signal(SIGINT, f);
  // signal(SIGINT, SIG_IGN);

  for (i = 0; i < 5; ++i)
  {
    printf("hello\n");
    sleep(2);
  }

  return 0;
}

void f(int signum)
{
  int c;
  printf("Interrupted! OK to quit (y/n)?");

  while((c = getchar()))
  {
    switch(c)
    {
      case 'n':
        return;
      case 'y':
        exit(1);
    }
  }
}