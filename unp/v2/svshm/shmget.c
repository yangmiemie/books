#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

#define SVSHM_MODE 0666

int main(int argc, char *argv[])
{
  int id, c, flag;

  flag = SVSHM_MODE | IPC_CREAT;

  while ((c = getopt(argc, argv, "e")) != -1)
  {
    switch(c)
    {
      case 'e':
        flag |= IPC_EXCL;
      break;
    }
  }

  if (argc - 2 != optind)
  {
    fprintf(stderr, "usage: shmget [ -e ] <pathname> <length>\n");
    exit(1);
  }

  if ((id = shmget(ftok(argv[optind], 0), atoi(argv[optind + 1]), flag)) < 0)
  {
    perror("shmget error");
    exit(1);
  }

  return 0;
}
