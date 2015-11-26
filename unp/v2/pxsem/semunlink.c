#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  sem_t *sem;

  if (argc != 2)
  {
    fprintf(stderr, "usage: semgetvalue <name>");
    exit(1);
  }

  if (sem_unlink(argv[1]) != 0)
  {
    perror("sem_unlink error");
    exit(1);
  }

  return 0;
}