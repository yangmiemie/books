#include "cliserv2.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_MODE 0666

int main(int argc, char const *argv[])
{
  struct shmstruct *ptr;
  int fd, i, offset, noverflow, lastoverflow, temp;

  if (argc != 2)
  {
    fprintf(stderr, "usage: server2 <name>\n");
    exit(1);
  }

  if ((shm_unlink(argv[1]) < 0))
  {
    perror("shm_unlink error");
    exit(1);
  }

  if ((fd = shm_open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0)
  {
    perror("shm_open error");
    exit(1);
  }

  if ((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == 0)
  {
    perror("mmap error");
    exit(1);
  }
  ftruncate(fd, sizeof(struct shmstruct));

  for (i = 0; i < NMSG; ++i)
  {
    (ptr -> msgoff)[i] = i * MSGSIZE;
  }

  sem_init(&ptr -> nstored, 1, 0);
  sem_init(&ptr -> nempty, 1, NMSG);
  sem_init(&ptr -> mutex, 1, 1);
  sem_init(&ptr -> noverflowmutex, 1, 1);

  i = 0;
  while (1)
  {
    sleep(2);

    sem_wait(&ptr -> nstored);
    sem_wait(&ptr -> mutex);
    offset = ptr -> msgoff[i];
    printf("index = %d: %s\n", i, &ptr -> msgdata[offset]);
    if (++i >= NMSG)
      i = 0;
    sem_post(&ptr -> mutex);
    sem_post(&ptr -> nempty);

    sem_wait(&ptr -> noverflowmutex);
    temp = ptr -> noverflow;
    sem_post(&ptr -> noverflowmutex);
    if (temp != lastoverflow)
    {
      printf("noverflow = %d\n", temp);
      lastoverflow = temp;
    }
  }

  return 0;
}