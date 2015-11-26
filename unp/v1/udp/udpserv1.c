#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 64

void dg_echo(int sockfd, struct sockaddr_in* cliaddr, int clilen)
{
  char recvline[MAXLINE], addr[MAXLINE];
  int n, port, len;
  struct sockaddr_in servaddr;

  len = sizeof(servaddr);
  if (getsockname(sockfd, (struct sockaddr*)&servaddr, &len) < 0)
  {
    fprintf(stderr, "getsockname error\n");
    exit(1);
  }

  if (inet_ntop(AF_INET, &servaddr.sin_addr.s_addr, addr, MAXLINE) == NULL)
  {
    perror("inet_ntop error");
    exit(1);
  }

  port = ntohs(servaddr.sin_port);

  printf("waiting connection from %s(%d)\n", addr, port);

  for ( ; ; )
  {
    if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, (struct sockaddr*)cliaddr, &clilen)) < 0)
    {
      perror("recvfrom error");
      exit(1);
    }

    recvline[n] = '\0';

    memset(addr, 0, MAXLINE);
    if (inet_ntop(AF_INET, &cliaddr -> sin_addr.s_addr, addr, MAXLINE) == NULL)
    {
      perror("inet_ntop error");
      exit(1);
    }

    port = ntohs(cliaddr -> sin_port);

    printf("receive from %s(%d): %s", addr, port, recvline);

    if ((n = sendto(sockfd, recvline, n, 0, (struct sockaddr*)cliaddr, clilen)) < 0)
    {
      perror("sendto error");
      exit(1);
    }
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