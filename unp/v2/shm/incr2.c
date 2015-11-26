#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define SEM_NAME "sem"
#define SEM_MODE 0666

struct shared {
  int count;
  sem_t sem;
} shared;

int main(int argc, char const *argv[])
{
  int i, nloops, fd, zero;
  sem_t* sem;
  int status;
  struct shared *ptr;

  if (argc != 3)
  {
    fprintf(stderr, "usage: incr2 <pathname> <#nloops>\n");
    exit(1);
  }

  if ((sem_init(&shared.sem, 1, 1)) < 0)
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
  write(fd, &shared, sizeof(struct shared));

  nloops = atoi(argv[2]);

  ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (fork() > 0)
  {
    for (i = 0; i < nloops; ++i)
    {
      sem_wait(&ptr -> sem);
      printf("parent: %d\n", ptr -> count++);
      sem_post(&ptr -> sem);
    }

    wait(&status);
  }
  else
  {
    for (i = 0; i < nloops; ++i)
    {
      sem_wait(&ptr -> sem);
      printf("child: %d\n", ptr -> count++);
      sem_post(&ptr -> sem);
    }
  }
  return 0;
}