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
    fprintf(stderr, "usage: shmread <pathname>\n");
    exit(1);
  }

  if ((id = shmget(ftok(argv[1], 0), 0, SVSHM_MODE)) < 0)
  {
    perror("shmget error");
    exit(1);
  }

  if (shmctl(id, IPC_STAT, &buf) < 0)
  {
    perror("shmctl error");
    exit(1);
  }

  if ((ptr = shmat(id, NULL, 0)) == NULL)
  {
    perror("shmat error");
    exit(1);
  }

  for (i = 0; i < buf.shm_segsz; ++i)
    printf("%d ", ptr[i]);

  printf("\n");
}