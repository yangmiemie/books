#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define SVSEM_MODE 0666
extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char const *argv[])
{
  int c;
  int flags;

  flags = SVSEM_MODE | IPC_CREAT;
  while ((c = getopt(argc, argv, "e")) != -1)
  {
    switch(c)
    {
      case 'e':
        flags |= IPC_EXCL;
      break;
    }
  }

  if (optind != argc - 2)
  {
    fprintf(stderr, "uasge: semcreate [ -e ] <pathname> <nsems>\n");
    exit(1);
  }

  if (semget(ftok(argv[1], 0), atoi(argv[2]), flags) == -1)
  {
    perror("semget error");
    exit(1);
  }

  return 0;
}