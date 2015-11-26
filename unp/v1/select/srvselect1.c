#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#define LISTENQ 16
#define MAXLINE 64

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

void sig_chld(int signo)
{
  int stat;
  int pid;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("%d teminated\n", pid);
}

void str_echo(int sockfd)
{
  struct sockaddr_in cliaddr;
  int len, n;
  char recvline[MAXLINE], addr[MAXLINE];
  int port;
  FILE *fp;
  int readBytes, writeBytes;

  len = sizeof(cliaddr);
  if (getpeername(sockfd, (struct sockaddr*)&cliaddr, &len) < 0)
  {
    perror("getpeername error");
    exit(1);
  }

  memset(addr, 0, MAXLINE);
  if (inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, addr, MAXLINE) == NULL)
  {
    perror("inet_ntop error");
    exit(1);
  }

  port = ntohs(cliaddr.sin_port);

  readBytes = writeBytes = 0;

  for (;;)
  {
    if ((n = read(sockfd, recvline, MAXLINE)) < 0)
    {
      perror("read error");
      exit(1);
    }
    else if (n == 0) 
    {
      printf("readBytes = %d\n", readBytes);
      printf("End of Socket\n");
      exit(1);
    }
    else 
    {
      readBytes += n;
      recvline[n] = '\0';
      // printf("%s", recvline);
      printf("receive from %s(%d): %s", addr, port, recvline);
      write(sockfd, recvline, strlen(recvline));
    }
  }
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in cliaddr, servaddr;
  int sockfd, connfd;
  int len;

  if (argc != 2)
  {
    fprintf(stderr, "usage: srvselect <port>\n");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind error");
    exit(1);
  }

  if (listen(sockfd, LISTENQ) < 0)
  {
    perror("listen error");
    exit(1);
  }

  Signal(SIGCHLD, sig_chld);

  for (;;)
  {
    len = sizeof(cliaddr);
    connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

    if (fork() == 0)
    {
      close(sockfd);
      str_echo(connfd);
    }

    close(connfd);
  }
  return 0;
}

