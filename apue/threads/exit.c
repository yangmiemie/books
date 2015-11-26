#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct foo {
  int a, b, c, d;
};

void printfoo(const char* s, const struct foo* fp)
{
  printf("%s\n", s);
  printf("  structure at 0x%x\n", (unsigned)fp);
  printf("  foo.a = %d\n", fp -> a);
  printf("  foo.b = %d\n", fp -> b);
  printf("  foo.c = %d\n", fp -> c);
  printf("  foo.d = %d\n", fp -> d);
}

void *thr_fn1(void* arg)
{
  struct foo foo = {1, 2, 3, 4};

  printfoo("thread 1: \n", &foo);
  pthread_exit((void*)&foo);
}

void *thr_fn2(void* arg)
{
  printf("thread 2: ID is %d\n", pthread_self);
  pthread_exit((void*)0);
}

int main(int argc, char const *argv[])
{
  int err;
  pthread_t tid1, tid2;
  struct foo* fp;

  if ((err = pthread_create(&tid1, NULL, thr_fn1, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create 1 error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_join(tid1, (void*)&fp)) != 0)
  {
    fprintf(stderr, "pthread_join 1 error: %s\n", strerror(err));
    exit(1);  
  }

  sleep(1);
  printf("parent starting second thread\n");

  if ((err = pthread_create(&tid2, NULL, thr_fn2, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create 2 error: %s\n", strerror(err));
    exit(1);
  }

  sleep(1);
  printfoo("parent: \n", fp);
  return 0;
}
