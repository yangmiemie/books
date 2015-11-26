#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SVSHM_MODE 0666

int main(int argc, char *argv[])
{
  int id, i;
  struct shmid_ds buf;
  unsigned char *ptr;

  if (argc != 2)
  {
    fprintf(stderr, "usage: shmwrite <pathname>\n");
    exit(1);
  }

  if ((id = shmget(ftok(argv[1], 0), 0, SVSHM_MODE)) < 0)
  {
    perror("shmrmid error");
    exit(1);
  }

  if (shmctl(id, IPC_STAT, &buf) < 0)
  {
    perror("shmctl error");
    exit(1);
  }

  if ((ptr = shmat(id, NULL, 0)) == -1)
  {
    perror("shmat error");
    exit(1);
  }

  printf("shm size: %d\n", buf.shm_segsz);

  for (i = 0; i < buf.shm_segsz; ++i)
  {
    *ptr++ = i % 256;
  }
}