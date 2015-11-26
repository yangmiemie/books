#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int global1 = 100;

int makethread(pthread_t *pthread, void* (*func)(void*), void* arg)
{
  int err;
  pthread_t tid;
  pthread_attr_t attr;

  if ((err = pthread_attr_init(&attr)) != 0)
  {
    fprintf(stderr, "pthread_attr_init error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)))
  {
    fprintf(stderr, "pthread_attr_setdetachstate error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_create(&tid, &attr, func, arg)) != 0)
  {
    fprintf(stderr, "pthread_create error: %s\n", strerror(err));
    exit(1);
  }

  *pthread = tid;
  pthread_attr_destroy(&attr);

  return err;
}

void *thr_fn1(void* arg)
{
  printf("thread 1 existing\n");
  pthread_exit((void*)&global1);
}

int main(int argc, char const *argv[])
{
  int err;
  pthread_t tid;
  void* tret;

  if ((makethread(&tid, thr_fn1, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create 1 error: %s\n", strerror(err));
    exit(1);
  }

  printf("tid = %d\n", tid);

  if ((err = pthread_join(tid, NULL)) != 0)
  {
    fprintf(stderr, "pthread_join 1 error: %s\n", strerror(err));
    exit(1);  
  }

  return 0;
}