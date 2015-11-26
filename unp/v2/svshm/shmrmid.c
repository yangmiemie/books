#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SVSHM_MODE 0666

int main(int argc, char *argv[])
{
  int id;

  if (argc != 2)
  {
    fprintf(stderr, "usage: shmrmid <pathname>\n");
    exit(1);
  }

  if ((id = shmget(ftok(argv[1], 0), 0, SVSHM_MODE)) < 0)
  {
    perror("shmget error");
    exit(1);
  }

  if (shmctl(id, IPC_RMID, NULL) < 0)
  {
    perror("shmctl error");
    exit(1);
  }
}