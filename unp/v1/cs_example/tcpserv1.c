#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#define LISTENQ 16
#define MAXLINE 64

extern int errno;
typedef void (*sighandler_t)(int);

void str_echo(int sockfd, struct sockaddr_in* cliaddr)
{
  int n;
  char buff[MAXLINE] = {0};
  char addr[MAXLINE] = {0};

  if (inet_ntop(AF_INET, &(cliaddr -> sin_addr), addr, MAXLINE) < 0)
  {
    perror("inet_ntop error");
    exit(1);
  }

  while ((n = read(sockfd, buff, MAXLINE)) > 0)
  {
    buff[n] = '\0';
    printf("receive from %s(%d): %s", addr, ntohs(cliaddr -> sin_port), buff);
    write(sockfd, buff, n);
  }

  if (n == 0)
  {
    printf("receive FIN\n");
  }
}

sighandler_t Signal(int signo, sighandler_t func)
{
  struct sigaction act, oact;
  sigset_t set;

  act.sa_handler = func;
  sigemptyset(&set);
  act.sa_mask = set;
  act.sa_flags = 0;
  act.sa_flags |= SA_NODEFER;  
  act.sa_flags |= SA_RESTART;

  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;

  return oact.sa_handler;
}

void sig_chld(int signo)
{
  int stat;
  pid_t pid;
  
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d teminated\n", pid);
  return;
}

int main(int argc, char const *argv[])
{
  int listenfd, connfd;
  struct sockaddr_in servaddr, cliaddr;
  int len;

  if (argc != 2)
  {
    fprintf(stderr, "usage: tcpserv <port>\n");
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

  Signal(SIGCHLD, sig_chld);

  while (1)
  {
    len = sizeof(cliaddr);
    // pause();
    if ((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len)) < 0)
    {
      if (errno == EINTR)
        continue;

      perror("accept error");
      exit(1);
    }

    if (fork() == 0)
    {
      close(listenfd);
      str_echo(connfd, &cliaddr);
      exit(0);
    }

    close(connfd);
  }
  return 0;
}