#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"

#define NBUFF 10
#define MAXTHREADS 100
#define MAXITEMS 1000000

int nitems;

struct {
  int buff[NBUFF];
  int nput;
  int nval;
  sem_t *mutex, *empty, *stored;
} shared;

void* produce();
void* consume();

#define FILE_MODE 0666

int main(int argc, char const *argv[])
{
  int flags, nthreads, i;
  pthread_t tid_produce[MAXTHREADS], tid_consume;

  if (argc != 3)
  {
    fprintf(stderr, "usage: prodcons1 <#item> <#threads>");
    exit(1);
  }

  nitems = atoi(argv[1]);
  nthreads = atoi(argv[2]);

  shared.nput = 0;
  shared.nval = 0;

  printf("nitems = %d, nthreads = %d\n", nitems, nthreads);

  flags = O_CREAT | O_RDWR;

  sem_unlink(SEM_MUTEX);
  sem_unlink(SEM_NEMPTY);
  sem_unlink(SEM_NSTORED);

  if ((shared.mutex = sem_open(SEM_MUTEX, flags, FILE_MODE, 1)) == SEM_FAILED)
  {
    fprintf(stderr, "sem open error");
    exit(1);
  }

  if ((shared.empty = sem_open(SEM_NEMPTY, flags, FILE_MODE, NBUFF)) == SEM_FAILED)
  {
    fprintf(stderr, "sem open error");
    exit(1);
  }

  if ((shared.stored = sem_open(SEM_NSTORED, flags, FILE_MODE, 0)) == SEM_FAILED)
  {
    fprintf(stderr, "sem open error");
    exit(1);
  }

  for (i = 0; i < nthreads; ++i)
  {
    pthread_create(&tid_produce[i], NULL, produce, NULL);
  }

  pthread_create(&tid_consume, NULL, consume, NULL);

  for (i = 0; i < nthreads; ++i)
  {
    pthread_join(tid_produce[i], NULL);
  }

  pthread_join(tid_consume, NULL);

  return 0;
}

void* consume()
{
  int i;

  // printf("in consume\n");
  for (i = 0; i < nitems; ++i)
  {
    sem_wait(shared.stored);
    sem_wait(shared.mutex);
    if (shared.buff[i % NBUFF] != i)
      printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
    sem_post(shared.mutex);
    sem_post(shared.empty);
  }
}

void* produce()
{
  while (1)
  {
    // printf("in produce\n");

    sem_wait(shared.empty);
    // printf("after produce empty wait\n");

    sem_wait(shared.mutex);

    // printf("after produce mutex wait\n");
    // printf("shared.nput = %d, nitems = %d\n", shared.nput, nitems);
    if (shared.nput >= nitems)
    {
      sem_post(shared.mutex);
      sem_post(shared.empty);
      return;
    }
    shared.buff[shared.nput % NBUFF] = shared.nval;
    shared.nval++;
    shared.nput++;
    sem_post(shared.mutex);
    sem_post(shared.stored);
  }
}
