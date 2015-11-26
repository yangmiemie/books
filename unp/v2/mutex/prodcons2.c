#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXITEMS 1000000
#define MAXTHREADS 100
#define MIN(a, b) (a > b ? b : a)

struct {
  pthread_mutex_t mutex;
  int buff[MAXITEMS];
  int nput;
  int nval;
} shared = {
  PTHREAD_MUTEX_INITIALIZER
};

int nitems;

void* produce(void*);
void* consume(void*);

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "usage: prodcons <#items> <#nthreads>\n");
    exit(1);
  }

  int nthreads, i, j;
  pthread_t* tid_produce, tid_consume;

  nitems = MIN(atoi(argv[1]), MAXITEMS);
  nthreads = MIN(atoi(argv[2]), MAXTHREADS);

  printf("nitems = %d, nthreads = %d\n", nitems, nthreads);

  tid_produce = malloc(nthreads * sizeof(pthread_t));

  for (i = 0; i < nthreads; ++i)
  {
    if (pthread_create(&tid_produce[i], NULL, produce, NULL) < 0)
    {
      perror("pthread_create error");
      exit(1);
    }
  }

  printf("create produce threads succeed\n");

  if (pthread_create(&tid_consume, NULL, consume, NULL) < 0)
  {
    perror("pthread_create consume error");
    exit(1);
  }

  for (i = 0; i < nthreads; ++i)
    pthread_join(tid_produce[i], NULL);

  printf("join produce threads succeed\n");

  pthread_join(tid_consume, NULL);

  printf("join consume threads succeed\n");

  return 0;
}


void* produce(void* args)
{
  while (1)
  {
    pthread_mutex_lock(&shared.mutex);
    if (shared.nput >= nitems)
    {
      pthread_mutex_unlock(&shared.mutex);
      return;
    }

    shared.buff[shared.nput] = shared.nval;
    shared.nput++;
    shared.nval++;

    pthread_mutex_unlock(&shared.mutex);
  }
}

void consume_wait(int i)
{
  while (1)
  {
    pthread_mutex_lock(&shared.mutex);
    // printf("consume get lock, nput = %d, i = %d\n", shared.nput, i);
    if (shared.nput > i)
    {
      pthread_mutex_unlock(&shared.mutex);
      return;
    }
    // printf("consume release lock, nput = %d, i = %d\n", shared.nput, i);
    pthread_mutex_unlock(&shared.mutex);
  }
}

void* consume(void* args)
{
  printf("in consume thread\n");
  int i;
  for (i = 0; i < nitems; ++i)
  {
    // consume_wait(i);
    if (i != shared.buff[i])
      printf("item %d: %d\n", i, shared.buff[i]);
      // printf("Error value found\n");
    // else
      // printf("item %d: %d\n", i, shared.buff[i]);
  }
}
