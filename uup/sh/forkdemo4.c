#include <stdio.h>

int main(int argc, char const *argv[])
{
  int i;
  for (i = 0; i < 3; ++i)
  {
    printf("my pid = %d, parent pid = %d, n = %d\n", getpid(), getppid(), i);

    sleep(1);

    if (fork() == 0)
    {
      exit(0);
    }
  }

  return 0;
}