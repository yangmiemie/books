#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int global1 = 100;
int global2 = 200;

void *thr_fn1(void* arg)
{
  printf("thread 1 existing\n");
  pthread_exit((void*)&global1);
}

void *thr_fn2(void* arg)
{
  printf("thread 2 existing\n");
  pthread_exit((void*)&global2);
}

int main(int argc, char const *argv[])
{
  int err;
  pthread_t tid1, tid2;
  void* tret;

  if ((err = pthread_create(&tid1, NULL, thr_fn1, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create 1 error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_create(&tid2, NULL, thr_fn2, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create 2 error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_join(tid1, &tret)) != 0)
  {
    fprintf(stderr, "pthread_join 1 error: %s\n", strerror(err));
    exit(1);  
  }
  printf("thread 1 exit code %d\n", *(int*)tret);

  if ((err = pthread_join(tid2, &tret)) != 0)
  {
    fprintf(stderr, "pthread_join 2 error: %s\n", strerror(err));
    exit(1);  
  }
  printf("thread 2 exit code %d\n", *(int*)tret);
  return 0;
}