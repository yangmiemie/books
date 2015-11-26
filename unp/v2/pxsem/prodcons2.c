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
  int nget;
  sem_t *mutex, *empty, *stored;
} shared;

void* produce();
void* consume();

#define FILE_MODE 0666

int main(int argc, char const *argv[])
{
  int flags, nproduce, nconsume, i;
  pthread_t tid_produce[MAXTHREADS], tid_consume[MAXTHREADS];

  if (argc != 4)
  {
    fprintf(stderr, "usage: prodcons1 <#item> <#produce> <#consume>\n");
    exit(1);
  }

  nitems = atoi(argv[1]);
  nproduce = atoi(argv[2]);
  nconsume = atoi(argv[3]);

  shared.nput = 0;
  shared.nval = 0;
  shared.nget = 0;

  printf("nitems = %d, nproduce = %d, nconsume = %d\n", nitems, nproduce, nconsume);

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

  for (i = 0; i < nproduce; ++i)
  {
    pthread_create(&tid_produce[i], NULL, produce, NULL);
  }
  for (i = 0; i < nconsume; ++i)
  {
    pthread_create(&tid_consume[i], NULL, consume, NULL);
  }

  for (i = 0; i < nproduce; ++i)
  {
    pthread_join(tid_produce[i], NULL);
  }
  printf("Done with produce\n");
  for (i = 0; i < nconsume; ++i)
  {
    pthread_join(tid_consume[i], NULL);
  }
  printf("Done with consume\n");

  // pthread_join(tid_consume, NULL);

  return 0;
}

void* consume()
{
  while(1)
  {
    sem_wait(shared.stored);
    sem_wait(shared.mutex);
    if (shared.nget >= nitems)
    {
      sem_post(shared.mutex);
      sem_post(shared.stored);
      return;
    }

    if (shared.buff[shared.nget % NBUFF] == shared.nget)
      printf("buff[%d] = %d\n", shared.nget, shared.buff[shared.nget % NBUFF]);

    shared.nget++;    
    sem_post(shared.mutex);
    sem_post(shared.empty);
  }
}

void* produce()
{
  while (1)
  {
    sem_wait(shared.empty);
    sem_wait(shared.mutex);
    if (shared.nput >= nitems)
    {
      sem_post(shared.mutex);
      sem_post(shared.empty);
      sem_post(shared.stored);
      return;
    }
    shared.buff[shared.nput % NBUFF] = shared.nval;
    shared.nval++;
    shared.nput++;
    sem_post(shared.mutex);
    sem_post(shared.stored);
  }
}
