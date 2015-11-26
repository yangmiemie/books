#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define SEM_NAME "sem"
#define SEM_MODE 0666

int count;
int main(int argc, char const *argv[])
{
  int i, nloops, fd, zero;
  sem_t* sem;
  int status;
  int *ptr;

  if (argc != 3)
  {
    fprintf(stderr, "usage: incr1 <pathname> <#nloops>\n");
    exit(1);
  }

  if ((sem = sem_open(SEM_NAME, O_CREAT, SEM_MODE, 1)) < 0)
  {
    perror("sem open error");
    exit(1);
  }

  if ((fd = open(argv[1], O_RDWR | O_CREAT, 0666)) < 0)
  {
    perror("open error");
    exit(1);
  }

  zero = 0;
  write(fd, &zero, sizeof(zero));

  nloops = atoi(argv[2]);

  ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (fork() > 0)
  {
    for (i = 0; i < nloops; ++i)
    {
      sem_wait(sem);
      printf("parent: %d\n", (*ptr)++);
      sem_post(sem);
    }

    wait(&status);
  }
  else
  {
    for (i = 0; i < nloops; ++i)
    {
      sem_wait(sem);
      printf("child: %d\n", (*ptr)++);
      sem_post(sem);
    }
  }
  return 0;
}