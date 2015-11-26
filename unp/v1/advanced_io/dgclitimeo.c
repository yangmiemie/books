#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

#define MAXLINE 64

typedef void (*sighandler_t)(int);

sighandler_t Signal(int signo, sighandler_t func)
{
  struct sigaction act, oact;
  sigset_t set;

  act.sa_handler = func;
  sigemptyset(&set);
  act.sa_mask = set;
  act.sa_flags = 0;
  act.sa_flags |= SA_NODEFER;  
  if (signo == SIGALRM)
    act.sa_flags &= ~SA_RESTART;

  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;

  return oact.sa_handler;
}

void sig_alrm(int signo)
{
  return;
}

void dg_cli(int sockfd, struct sockaddr_in* servcaddr, int servlen)
{
  char recvline[MAXLINE], sendline[MAXLINE], addr[MAXLINE];
  int n, port;
  sighandler_t sigfunc;

  if (inet_ntop(AF_INET, &servcaddr -> sin_addr.s_addr, addr, MAXLINE) == NULL)
  {
    perror("inet_ntop error");
    exit(1);
  }

  port = ntohs(servcaddr -> sin_port);

  Signal(SIGALRM, sig_alrm);

  while (fgets(recvline, MAXLINE, stdin) != NULL)
  {
    if ((n = sendto(sockfd, recvline, strlen(recvline), 0, (struct sockaddr*)servcaddr, servlen)) < 0)
    {
      perror("sendto error");
      exit(1);
    }

    alarm(5);
    if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0)
    {
      if (errno == EINTR)
      {
        fprintf(stderr, "socket recvfrom timeout\n");
      }
      else 
      {
        perror("recvfrom error");
      }
      
      exit(1);
    }
    // alarm(0);
    recvline[n] = '\0';
    printf("%s", recvline);
  }

}

int main(int argc, char const *argv[])
{
  struct sockaddr_in servcaddr;
  int sockfd;

  if (argc != 3)
  {
    fprintf(stderr, "usage: udpcli <addr> <port>\n");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  servcaddr.sin_family = AF_INET;
  servcaddr.sin_port = htons(atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], &servcaddr.sin_addr) < 0)
  {
    perror("inet_pton error");
    exit(1);
  }

  dg_cli(sockfd, &servcaddr, sizeof(servcaddr));
  return 0;
}