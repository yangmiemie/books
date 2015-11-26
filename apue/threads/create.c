#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

pthread_t ntid;

void printids(const char* s)
{
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();
  printf("ntid = (0x%x), pthread_equal = %d\n", tid, pthread_equal(tid, ntid));
  printf("%s pid %u tid %u (0x%x)\n", s, pid, tid, tid);
}

void *thr_fn(void *arg)
{
  int val;

  val = *((int*)arg);
  printf("value from main thread: %d\n", val);
  printids("new thread : ");
  return (void*)0;
}

int main(int argc, char const *argv[])
{
  int err;
  int val;

  val = 10000;
  if ((err = pthread_create(&ntid, NULL, thr_fn, &val)) != 0)
  {
    fprintf(stderr, "pthread_create error: %s\n", strerror(err));
    exit(1);
  }

  printids("main thread: ");
  sleep(1);
  return 0;
}