#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXITEMS 1000000
#define MAXTHREADS 100
#define MIN(a, b) (a > b ? b : a)

int buff[MAXITEMS];

struct {
  pthread_mutex_t mutex;
  int nput;
  int nval;
} shared = {
  PTHREAD_MUTEX_INITIALIZER
};

struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int nready;
} nready = {
  PTHREAD_MUTEX_INITIALIZER,
  PTHREAD_COND_INITIALIZER
};

int nitems;
int count;
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
  printf("count = %d\n", count);
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

    buff[shared.nput] = shared.nval;
    shared.nput++;
    shared.nval++;

    pthread_mutex_unlock(&shared.mutex);

    pthread_mutex_lock(&nready.mutex);
    if (nready.nready == 0)
    {
      pthread_cond_signal(&nready.cond);
      ++count;      
    }

    nready.nready++;
    pthread_mutex_unlock(&nready.mutex);
  }
}

void* consume(void* args)
{
  printf("in consume thread\n");
  int i;
  for (i = 0; i < nitems; ++i)
  {
    pthread_mutex_lock(&nready.mutex);
    while (nready.nready == 0)
      pthread_cond_wait(&nready.cond, &nready.mutex);

    // if (i != buff[i])
      printf("item %d: %d\n", i, buff[i]);
    nready.nready--;

    pthread_mutex_unlock(&nready.mutex);
  }
}
