#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define SVSEM_MODE 0666

union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

int main(int argc, char const *argv[])
{
  int i, semid, semnums;
  struct semid_ds seminfo;
  unsigned short *ptr;
  union semun arg;

  if (argc != 2)
  {
    fprintf(stderr, "uasge: semgetvalues <pathname>\n");
    exit(1);
  }

  if ((semid = semget(ftok(argv[1], 0), 0, 0)) == -1)
  {
    perror("semget error");
    exit(1);
  }

  arg.buf = &seminfo;
  if (semctl(semid, 0, IPC_STAT, arg) == -1)
  {
    perror("semctl stat error");
    exit(1);
  }

  semnums = seminfo.sem_nsems;
  ptr = calloc(semnums, sizeof(unsigned short));
  arg.array = ptr;

  if (semctl(semid, 0, GETALL, arg) == -1)
  {
    perror("semctl getall error");
    exit(1);
  }
 
  for (i = 0; i < semnums; ++i)
    printf("semval[%d] = %d\n", i, ptr[i]);

  return 0;
}