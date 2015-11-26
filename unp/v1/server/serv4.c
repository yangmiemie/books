#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/mman.h>
#include <pthread.h>

#define LISTENQ 16
#define MAXN 16384
#define MAXLINE 64

void web_child(int sockfd)
{
  int ntowrite, nread;
  char line[MAXLINE], result[MAXN];
  pthread_t tid;

  memset(line, 0, MAXLINE);
  tid = pthread_self();

  for ( ; ; )
  {
    if ((nread = read(sockfd, line, MAXLINE - 1)) == 0)
    {
      printf("thread %d receive read = 0\n", tid);
      return;      
    }

    ntowrite = atoi(line);
    printf("thread %d write %d bytes\n", tid, ntowrite);

    write(sockfd, result, ntowrite);
  }
}

typedef void (*sighandler_t)(int);

sighandler_t Signal(int signo, sighandler_t handler)
{
  struct sigaction act, oact;
  sigset_t set;

  sigemptyset(&set);
  act.sa_handler = handler;
  act.sa_flags = 0;
  act.sa_flags |= SA_RESTART;
  act.sa_mask = set;

  if (sigaction(signo, &act, &oact) < 0)
  {
    perror("sigaction error");
    exit(1);
  }

  return oact.sa_handler;
}

void pr_cpu_time(void)
{
  double user, sys;
  struct rusage myusage, childusage;

  if (getrusage(RUSAGE_SELF, &myusage) < 0)
  {
    perror("getrusage error");
    exit(1);
  }

  if (getrusage(RUSAGE_CHILDREN, &childusage) < 0)
  {
    perror("getrusage error");
    exit(1);
  }

  user = (double) myusage.ru_utime.tv_sec +
     myusage.ru_utime.tv_usec / 1000000.0;
  user += (double) childusage.ru_utime.tv_sec +
     childusage.ru_utime.tv_usec / 1000000.0;
  sys = (double) myusage.ru_stime.tv_sec +
     myusage.ru_stime.tv_usec / 1000000.0;
  sys += (double) childusage.ru_stime.tv_sec +
     childusage.ru_stime.tv_usec / 1000000.0;

  printf("\nuser time = %g, sys time = %g\n", user, sys);
}

void sig_int(int signo)
{
  pr_cpu_time();
  exit(0);
}

void* doit(void *arg)
{
  int connfd;

  connfd = ((int)arg);

  pthread_detach(pthread_self());
  web_child(connfd);

  // printf("pthread id = %d, connfd = %d\n", pthread_self(), connfd);
  close(connfd);
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in cliaddr, servaddr;
  int listenfd, connfd;
  int clilen, addrlen;
  int i;
  pthread_t tid;

  if (argc != 2)
  {
    fprintf(stderr, "usage: serv1 <#port>\n");
    exit(1);
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);

  if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind error");
    exit(1);
  }

  if (listen(listenfd, LISTENQ) < 0)
  {
    perror("listen error");
    exit(1);
  }

  Signal(SIGINT, sig_int);

  for ( ; ; )
  {
    connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

    if (pthread_create(&tid, NULL, &doit, (void*)connfd) != 0)
    {
      perror("pthread_create error");
      exit(1);
    }
  }

  return 0;
}