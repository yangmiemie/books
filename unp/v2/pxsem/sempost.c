#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  sem_t *sem;
  int value;

  if (argc != 2)
  {
    fprintf(stderr, "usage: semgetvalue <name>");
    exit(1);
  }

  if ((sem = sem_open(argv[1], 0)) == SEM_FAILED)
  {
    perror("sem open error");
    exit(1);
  }
  
  if (sem_getvalue(sem, &value) != 0)
  {
    perror("sem_getvalue error");
    exit(1);
  }

  printf("%d\n", value);

  if (sem_post(sem) != 0)
  {
    perror("sem post");
    exit(1);
  }

  if (sem_getvalue(sem, &value) != 0)
  {
    perror("sem_getvalue error");
    exit(1);
  }

  printf("%d\n", value);

  return 0;
}