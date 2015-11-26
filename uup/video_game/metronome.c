#include <signal.h>
#include <stdlib.h>

int* pids;
int timeeval;
int main(int argc, char const *argv[])
{
  int size = argc - 2;
  int i;
  timeeval = atoi(*++argv);
  pids = (int*)malloc(sizeof(int) * size);

  for (i = 0; i < size; ++i)
  {
    pids[i] = atoi(*++argv);
  }

  while(1)
  {
    sleep(1);
    for (i = 0; i < size; ++i)
    {
      kill(pids[i], SIGINT);
    }
  }
  return 0;
}