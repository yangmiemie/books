#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

struct shmstruct {
  int count;
};

#define FILE_MODE 0666

int main(int argc, char const *argv[])
{
  int shmid;
  struct shmstruct *ptr;
  sem_t *sem;
  int i, loops, pid;

  if (argc != 4)
  {
    fprintf(stderr, "usage: server1 <shmname> <semname> <loops>\n");
    exit(1);
  } 

  if ((shmid = shm_open(argv[1], O_RDWR, FILE_MODE)) < 0)
  {
    perror("shm_open error");
    exit(1);
  }

  if ((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) == NULL)
  {
    perror("mmap error");
    exit(1);
  }

  if ((sem = sem_open(argv[2], O_RDWR)) == NULL)
  {
    perror("sem_open error");
    exit(1);
  }

  loops = atoi(argv[3]);

  for (i = 0; i < loops; ++i)
  {
    sem_wait(sem);
    printf("pid %d: %d\n", pid, ptr -> count);
    ptr -> count++;
    sem_post(sem);
  }

  // shm_unlink(argv[1]);
  sem_close(sem);

  return 0;
}