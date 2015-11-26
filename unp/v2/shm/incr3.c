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
  int i, nloops, fd;
  sem_t* sem;
  int status;
  struct shared *ptr;

  if (argc != 2)
  {
    fprintf(stderr, "usage: incr3 <#nloops>\n");
    exit(1);
  }

  if ((sem_init(&shared.sem, 1, 1)) < 0)
  {
    perror("sem open error");
    exit(1);
  }

  nloops = atoi(argv[1]);

  fd = open("/dev/zero", O_RDWR);

  ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  // ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
  ptr -> sem = shared.sem;

  printf("%x\n", ptr);

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