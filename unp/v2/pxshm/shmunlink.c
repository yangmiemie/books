#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: shmunlink <name>\n");
    exit(1);
  }

  if (shm_unlink(argv[1]) < 0)
  {
    perror("shm_unlink error");
    exit(1);
  }

  return 0;
}
