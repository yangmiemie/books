#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#define MAXLINE 64

typedef void (*sighandler_t)(int);

sighandler_t Signal(int signo, sighandler_t handler)
{
  struct sigaction oact, act;
  sigset_t mask;

  sigemptyset(&mask); 
  act.sa_handler = handler;
  act.sa_mask = mask;

  if (signo == SIGALRM)
    act.sa_flags &= ~SA_RESTART;

  if (sigaction(signo, &act, &oact) < 0)
    return NULL;

  return oact.sa_handler;
}

void sig_alrm(int signo)
{
  return;
}

void dg_cli(int sockfd, struct sockaddr_in* servcaddr, int servlen)
{
  char recvline[MAXLINE], sendline[MAXLINE], addr[MAXLINE];
  struct sockaddr_in *preply_addr;
  int n, len, on, maxfd;
  sigset_t mask, empty;
  fd_set rset;

  preply_addr = malloc(sizeof(struct sockaddr_in));

  on = 1;
  // if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == -1)
  // {
  //   perror("setsockopt error");
  //   exit(1);
  // }

  Signal(SIGALRM, sig_alrm);


  sigemptyset(&mask);
  sigemptyset(&empty);
  sigaddset(&mask, SIGALRM);

  maxfd = sockfd;
  FD_ZERO(&rset);
  FD_SET(sockfd, &rset);

  while ((n = read(0, sendline, MAXLINE)) != 0)
  {

    sendline[n] = '\0';
    if((sendto(sockfd, sendline, n, 0, (struct sockaddr*)servcaddr, servlen) < 0))
    {
      perror("sendto error");
      exit(1);
    }

    alarm(5);

    for ( ; ; )
    {
      sigprocmask(SIG_BLOCK, &mask, 0);

      n = pselect(maxfd + 1, &rset, NULL, NULL, NULL, &empty);
      if (n < 0)
      {
        if (errno == EINTR)
        {
          printf("timeout\n");
          break;          
        }
        else
        {
          perror("recvfrom error");
          exit(1);
        }
      }
      else if (n != 1) 
      {
        fprintf(stderr, "pselect return error %d\n", n);
        exit(1);
      }
      else
      {
        n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr*)preply_addr, &len);
        recvline[n] = '\0';
        memset(addr, 0, MAXLINE);
        if (inet_ntop(AF_INET, &(preply_addr -> sin_addr.s_addr), addr, MAXLINE) == NULL)
        {
          perror("inet_ntop error");
          exit(1);
        }

        printf("from %s: %s", addr, recvline);
      }
    }
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