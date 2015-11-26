#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MAXLEN 64
#define LISTENQ 16

void str_echo(int sockfd);
void getsockinfo(struct sockaddr_in* infoaddr, char* addr, int len, int* port);

typedef void (*sighandler_t)(int);

sighandler_t Signal(int signo, sighandler_t handler)
{
  struct sigaction act, oact;
  sigset_t mask;

  sigemptyset(&mask);
  act.sa_handler = handler;
  act.sa_flags = 0;
  act.sa_flags |= SA_RESTART;
  act.sa_mask = mask;

  if (sigaction(signo, &act, &oact) < 0)
  {
    perror("sigaction error");
    exit(1);
  }

  return oact.sa_handler;
}

void sig_chld(int signo)
{
  pid_t pid;
  int status;
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    printf("child %d teminated\n", pid);
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in cliaddr, servaddr;
  int listenfd, connfd, udpfd, maxfd;
  fd_set rset, allset;
  int nready, len, n;
  char recvline[MAXLEN], ip[MAXLEN];
  int port;
  pid_t pid;

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

  if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("udp socket error");
    exit(1);
  }

  if (bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind error");
    exit(1);
  }

  Signal(SIGCHLD, sig_chld);

  FD_ZERO(&allset);
  FD_SET(listenfd, &allset);
  FD_SET(udpfd, &allset);

  maxfd = MAX(listenfd, udpfd);

  for ( ; ; )
  {
    rset = allset;

    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

    if (FD_ISSET(listenfd, &rset))
    {
      len = sizeof(cliaddr);
      connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
      printf("tcp connection established, connfd = %d\n", connfd);

      if ((pid = fork()) < 0)
      {
        perror("fork error");
        exit(1);
      }
      else if (pid == 0)
      {
        close(listenfd);
        str_echo(connfd);
        exit(1);
      }

      close(connfd);

      if (--nready <= 0)
        continue;
    }

    if (FD_ISSET(udpfd, &rset))
    {
      n = recvfrom(udpfd, recvline, MAXLEN, 0, (struct sockaddr*)&cliaddr, &len);
      recvline[n] = '\0';
      getsockinfo(&cliaddr, ip, MAXLEN, &port);
      printf("recvfrom %s(%d): %s", ip, port, recvline);
      sendto(udpfd, recvline, n, 0, (struct sockaddr*)&cliaddr, len);
    }
  }
}

void getsockinfo(struct sockaddr_in* infoaddr, char* addr, int len, int* port)
{
  memset(addr, 0, len);
  if (inet_ntop(AF_INET, &infoaddr -> sin_addr, addr, len) < 0)
  {
    perror("inet_ntop error");
    exit(1);
  }

  *port = ntohs(infoaddr -> sin_port);

  return;
}

void str_echo(int sockfd)
{
  char recvline[MAXLEN], ip[MAXLEN];
  int n, port, len;
  struct sockaddr_in cliaddr;

  len = sizeof(cliaddr);
  if (getpeername(sockfd, (struct sockaddr*)&cliaddr, &len) < 0)
  {
    perror("getpeername error");
    exit(1);
  }

  while ((n = read(sockfd, recvline, MAXLEN)) > 0)
  {
    recvline[n] = '\0';
    getsockinfo(&cliaddr, ip, MAXLEN, &port);
    printf("recvfrom %s(%d): %s", ip, port, recvline);
    write(sockfd, recvline, n);
  }
}


