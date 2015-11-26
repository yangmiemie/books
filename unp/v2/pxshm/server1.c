#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_MODE 0666

struct shmstruct {
  int count;
};

int main(int argc, char const *argv[])
{
  int shmid;
  struct shmstruct *ptr;
  sem_t *sem;

  if (argc != 3)
  {
    fprintf(stderr, "usage: server1 <shmname> <semname>\n");
    exit(1);
  } 

  if ((shmid = shm_open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0)
  {
    perror("shm_open error");
    exit(1);
  }

  ftruncate(shmid, sizeof(struct shmstruct));

  if ((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0)) == NULL)
  {
    perror("mmap error");
    exit(1);
  }

  // shm_unlink(argv[1]);

  sem_unlink(argv[2]);
  if ((sem = sem_open(argv[2], O_RDWR | O_CREAT, FILE_MODE, 1)) == NULL)
  {
    perror("sem_open error");
    exit(1);
  }

  sem_close(sem);
  return 0;
}