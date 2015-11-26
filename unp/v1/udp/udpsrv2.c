#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define MAXLEN 30000
int count;

void sig_int(int signo)
{
  printf("\nreceived %d datagrams\n", count);
  exit(1);
}

void dg_echo(int sockfd, struct sockaddr_in* cliaddr, int clilen)
{
  int len, bufsize;
  char mesg[MAXLEN];

  signal(SIGINT, sig_int);

  len = sizeof(bufsize);
  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, &len) < 0)
  {
    perror("getsockopt error");
    exit(1);
  }

  printf("SO_RCVBUF = %d\n", bufsize);
  bufsize *= 32;
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, len) < 0)
  {
    perror("getsockopt error");
    exit(1);
  }

  for ( ; ; )
  {
    recvfrom(sockfd, mesg, MAXLEN, 0, (struct sockaddr*)cliaddr, &clilen);

    count++;
  }
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in servaddr, cliaddr;
  int sockfd;

  if (argc != 2)
  {
    fprintf(stderr, "usage: udpserv <port>\n");
    exit(1);
  }
  
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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

  dg_echo(sockfd, &cliaddr, sizeof(cliaddr));
  return 0;
}