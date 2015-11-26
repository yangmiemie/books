#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cleanup(void* arg)
{
	printf("cleanup: %s\n", (char*)arg);
}

void* thr_fn1(void *arg)
{
	printf("thread 1 start\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	printf("thread 1 push complete\n");

	if (arg)
		return (void*)1;

	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	return (void*)1;
}

void* thr_fn2(void *arg)
{
	printf("thread 2 start\n");
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");
	printf("thread 2 push complete\n");

	// sleep(3);
	// if (arg)
	//   pthread_exit((void*)2);

	pthread_cleanup_pop(2);
	pthread_cleanup_pop(4);

	return (void*)2;
}

int main(int argc, char const *argv[])
{
  int err;
  pthread_t tid1, tid2;
  void* tret;

  if ((err = pthread_create(&tid1, NULL, thr_fn1, (void*)1)) != 0)
  {
    fprintf(stderr, "pthread_create 1 error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_create(&tid2, NULL, thr_fn2, (void*)2)) != 0)
  {
    fprintf(stderr, "pthread_create 2 error: %s\n", strerror(err));
    exit(1);
  }

  if ((err = pthread_join(tid1, &tret)) != 0)
  {
    fprintf(stderr, "pthread_join 1 error: %s\n", strerror(err));
    exit(1);  
  }
  printf("thread 1 exit code %d\n", (int)tret);

  // pthread_cancel(tid2);

  if ((err = pthread_join(tid2, &tret)) != 0)
  {
    fprintf(stderr, "pthread_join 2 error: %s\n", strerror(err));
    exit(1);  
  }
  printf("thread 2 exit code %d\n", (int)tret);	
	return 0;
}