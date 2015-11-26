#include "cliserv2.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern int errno;
#define FILE_MODE 0666

int main(int argc, char const *argv[])
{
  int fd, i, nloops, seconds, offset;
  struct shmstruct *ptr;
  pid_t pid;
  char msg[MSGSIZE];

  if (argc != 4)
  {
    fprintf(stderr, "usage: client2 <name> <loops> <seconds>\n");
    exit(1);
  }

  nloops = atoi(argv[2]);
  seconds = atoi(argv[3]);

  if ((fd = shm_open(argv[1], O_RDWR, FILE_MODE)) < 0)
  {
    perror("shm_open error");
    exit(1);
  }

  if ((ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
  {
    perror("mmap error");
    exit(1);
  }

  pid = getpid();
  for (i = 0; i < nloops; ++i)
  {
    sleep(seconds);
    snprintf(msg, MSGSIZE, "pid %d: message %d", pid, i);
    printf("%s\n", msg);
    if (sem_trywait(&ptr -> nempty) == -1)
    {
      if (errno == EAGAIN)
      {
        sem_wait(&ptr -> noverflowmutex);
        ptr -> noverflow++;
        sem_post(&ptr -> noverflowmutex);
        continue;
      }
      else
      {
        fprintf(stderr, "sem_trywait error\n");
        exit(1);
      }
    }

    sem_wait(&ptr -> mutex);
    offset = ptr -> msgoff[ptr -> nput];
    if (++(ptr -> nput) >= NMSG)
      ptr -> nput = 0;
    sem_post(&ptr -> mutex);

    strcpy(&ptr -> msgdata[offset], msg);
    sem_post(&ptr -> nstored);
  }
}