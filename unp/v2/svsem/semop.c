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
  int c, i, semid;
  struct sembuf *ptr;
  int flag, nops;

  // flag = SEM_UNDO;
  // while ((c = getopt(argc, argv, "n")) != -1)
  // {
  //   switch(c)
  //   {
  //     case 'n':
  //       flag |= IPC_NOWAIT;
  //     break;
  //   }
  // }

  optind++;
  if (argc < optind + 1)
  {
    fprintf(stderr, "usage: semops [ -e ] <pathname> operation\n");
    exit(1);
  }

  nops = argc - optind;
  ptr = calloc(nops, sizeof(struct sembuf));

  for (i = 0; i < nops; ++i)
  {
    ptr[i].sem_num = i;
    ptr[i].sem_op = atoi(argv[2 + i]);
    ptr[i].sem_flg = flag;
  }

  if ((semid = semget(ftok(argv[1], 0), 0, 0)) == -1)
  {
    perror("semget error");
    exit(1);
  }

  if (semop(semid, ptr, nops) == -1)
  {
    perror("semop error");
    exit(1);
  }

  return 0;
}