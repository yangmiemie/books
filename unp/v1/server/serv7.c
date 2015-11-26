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
#include <sys/select.h>

#define LISTENQ 16
#define MAXN 16384
#define MAXLINE 64
#define STDERR_FILENO 2

typedef struct {
  pid_t child_pid;
  int child_fd;
  int child_status;
} Child;

Child *cptr;

void web_child(int i, int sockfd)
{
  int ntowrite, nread;
  char line[MAXLINE], result[MAXN];

  memset(line, 0, MAXLINE);

  for ( ; ; )
  {
    if ((nread = read(sockfd, line, MAXLINE)) == 0)
      return;

    ntowrite = atoi(line);
    // printf("child %d write %d bytes\n", i, ntowrite);

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

void sig_chld(int signo)
{
  int stat;
  int pid;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("%d teminated\n", pid);
}

void child_main(int i)
{
  int connfd, n;

  for ( ; ; )
  {
    if ((n = read(STDERR_FILENO, &connfd, sizeof(connfd))) == 0)
    {
      perror("child_main read error");
      exit(1);
    }

    printf("child %d receive connfd = %d\n", i, connfd);
    web_child(i, connfd);
    close(connfd);
    write(STDERR_FILENO, "", 1);
  }
}

int child_make(int i, int listenfd)
{
  pid_t pid;
  int sockfd[2];

  if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0)
  {
    perror("socketpair error");
    exit(1);
  }

  if ((pid = fork()) > 0)
  {
    cptr[i].child_pid = pid;
    cptr[i].child_status = 0;
    cptr[i].child_fd = sockfd[0];
    close(sockfd[1]);
    return pid;
  }

  dup2(sockfd[1], STDERR_FILENO);
  close(sockfd[0]);
  close(sockfd[1]);
  close(listenfd);

  child_main(i);
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in cliaddr, servaddr;
  int listenfd, connfd, maxfd;
  int clilen, addrlen;
  int nchildren, i, nready, navail, n;
  fd_set rset, allset;
  int rc;

  if (argc != 3)
  {
    fprintf(stderr, "usage: serv1 <#port> <#nchildren>\n");
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

  nchildren = atoi(argv[2]);

  cptr = calloc(nchildren, sizeof(Child));

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

  FD_ZERO(&allset);

  maxfd = listenfd;
  FD_SET(listenfd, &allset);

  for (i = 0; i < nchildren; ++i)
  {
    child_make(i, listenfd);
    FD_SET(cptr[i].child_fd, &allset);
    maxfd = maxfd > cptr[i].child_fd ? maxfd : cptr[i].child_fd;
  }

  Signal(SIGCHLD, sig_chld);
  Signal(SIGINT, sig_int);

  addrlen = sizeof(servaddr);
  navail = nchildren;

  for ( ; ; )
  {
    rset = allset;

    if (navail <= 0)
      FD_CLR(listenfd, &rset);

    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

    if (FD_ISSET(listenfd, &rset))
    {
      clilen = addrlen;

      if ((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
      {
        perror("accept error");
        exit(1);
      }

      for (i = 0; i < nchildren; ++i)
        if (cptr[i].child_status == 0)
          break;

      if (i == nchildren)
      {
        fprintf(stderr, "out of children\n");
        exit(1);
      }

      cptr[i].child_status = 1;
      --navail;

      printf("connfd = %d\n", connfd);
      write(cptr[i].child_fd, &connfd, sizeof(connfd));

      close(connfd);
      if (--nready < 0)
        continue;
    }

    for (i = 0; i < nchildren; ++i)
    {
      if (FD_ISSET(cptr[i].child_fd, &rset))
      {
        if ((n = read(cptr[i].child_fd, &rc, 1)) == 0)
        {
          perror("read error");
          exit(1);
        }

        ++navail;
        cptr[i].child_status = 0;

        if (--nready <= 0)
          break;
      }
    }

  }

  return 0;
}

