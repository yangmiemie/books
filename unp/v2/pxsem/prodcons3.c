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
  int nget;
  int nready;
} nready = {
  PTHREAD_MUTEX_INITIALIZER,
  PTHREAD_COND_INITIALIZER
};

int nitems;
int count;
void* produce(void*);
void* consume(void* arg);

int main(int argc, char const *argv[])
{
  if (argc != 4)
  {
    fprintf(stderr, "usage: prodcons <#items> <#nproduce> <#nconsume>\n");
    exit(1);
  }

  int nproduce, nconsume, i, j;
  pthread_t* tid_produce, *tid_consume;

  nitems = MIN(atoi(argv[1]), MAXITEMS);
  nproduce = MIN(atoi(argv[2]), MAXTHREADS);
  nconsume = MIN(atoi(argv[3]), MAXTHREADS);

  printf("nitems = %d, nproduce = %d, nconsume = %d\n", nitems, nproduce, nconsume);

  tid_produce = malloc(nproduce * sizeof(pthread_t));
  tid_consume = malloc(nconsume * sizeof(pthread_t));

  for (i = 0; i < nproduce; ++i)
  {
    if (pthread_create(&tid_produce[i], NULL, produce, NULL) < 0)
    {
      perror("pthread_create error");
      exit(1);
    }
  }

  for (i = 0; i < nconsume; ++i)
  {
    if (pthread_create(&tid_consume[i], NULL, consume, &i) < 0)
    {
      perror("pthread_create error");
      exit(1);
    }
  }

  printf("create produce threads succeed\n");

  for (i = 0; i < nproduce; ++i)
    pthread_join(tid_produce[i], NULL);

  printf("join produce threads succeed\n");

  for (i = 0; i < nconsume; ++i)
    pthread_join(tid_consume[i], NULL);
  // pthread_join(tid_consume, NULL);

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
      pthread_cond_broadcast(&nready.cond);
      ++count;      
    }

    nready.nready++;
    pthread_mutex_unlock(&nready.mutex);
  }
}

void* consume(void* args)
{
  printf("in consume thread\n");
  int id;

  id = *((int*)args);

  while (1)
  {
    pthread_mutex_lock(&nready.mutex);
    if (nready.nget >= nitems)
    {
      pthread_cond_broadcast(&nready.cond);
      pthread_mutex_unlock(&nready.mutex);
      return;
    }

    printf("%d: get nready lock, nready = %d\n", id, nready.nready);
    while (nready.nready == 0)
      pthread_cond_wait(&nready.cond, &nready.mutex);
    nready.nready--;
    nready.nget++;
    pthread_mutex_unlock(&nready.mutex);
  }
}
