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
#include <semaphore.h>

#define LISTENQ 16
#define MAXN 16384
#define MAXLINE 64

int nchildrens;
pid_t *pids;

struct shared {
  sem_t mutex;
} shared;

struct shared *ptr;

void lock_wait()
{
  sem_wait(&ptr -> mutex);
}

void lock_release()
{
  sem_post(&ptr -> mutex);
}

void web_child(int i, int sockfd)
{
  int ntowrite, nread;
  char line[MAXLINE], result[MAXN];

  memset(line, 0, MAXLINE);

  for ( ; ; )
  {
    if ((nread = read(sockfd, line, MAXLINE - 1)) == 0)
    {
      printf("receive read = 0\n");
      return;      
    }

    ntowrite = atoi(line);
    printf("child %d write %d bytes\n", i, ntowrite);

    write(sockfd, result, ntowrite);
  }
}

void sig_chld(int signo)
{
  int stat;
  int pid;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("%d teminated\n", pid);
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
  printf("%d sig_int catch\n", getpid());
  int i;

  for (i = 0; i < nchildrens; ++i)
  {
    printf("pids[%d] = %d\n", i, pids[i]);
    kill(pids[i], SIGTERM);    
  }

  while (wait(NULL) > 0)
    ;

  pr_cpu_time();
  exit(0);
}

void child_main(int i, int sockfd)
{
  int connfd;
  struct sockaddr_in cliaddr;
  int clilen;

  printf("child %d starting\n", i);

  for ( ; ; )
  {
    lock_wait();
    connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
    lock_release();
    
    web_child(i, connfd);

    printf("%d done one job\n", i);
    close(connfd);
  }
}

pid_t child_make(int i, int listenfd)
{
  pid_t pid;

  if ((pid = fork()) > 0)
  {
    printf("child %d = %d\n", i, pid);
    return pid;    
  }

  child_main(i, listenfd);
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in cliaddr, servaddr;
  int listenfd, connfd;
  int clilen, addrlen;
  int i;

  if (argc != 3)
  {
    fprintf(stderr, "usage: serv1 <#port> <nchildrens>\n");
    exit(1);
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  nchildrens = atoi(argv[2]);

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

  pids = calloc(nchildrens, sizeof(pid_t));

  ptr = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
  sem_init(&ptr -> mutex, 1, 1);

  for (i = 0; i < nchildrens; ++i)
  {
    pids[i] = child_make(i, listenfd);
    printf("child_make pids[%d] = %d\n", i, pids[i]);    
  }

  Signal(SIGCHLD, sig_chld);
  Signal(SIGINT, sig_int);

  for ( ; ; )
    pause();

  return 0;
}