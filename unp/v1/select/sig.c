#include <signal.h>

void sig_int(int signo)
{
  printf("catch SIGINT\n");
}

int main(int argc, char const *argv[])
{
  int i;
  signal(SIGINT, sig_int);
  while (1)
    i = 0;

  return 0;
}