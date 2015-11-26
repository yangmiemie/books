#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 64

int main(int argc, char const *argv[])
{
  int sockfd, port, n;
  struct sockaddr_in servaddr, cliaddr;
  char ip[] = "127.0.0.1";
  char recvline[MAXLINE + 1] = {0};
  int len;

  if (argc != 3)
  {
    fprintf(stderr, "usage: daytimetcpcli <ip> <port>\n");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  port = atoi(argv[2]);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if ((inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0)
  {
    perror("inet_pton error");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("connect error");
    exit(1);
  }

  len = sizeof(cliaddr);
  if (getsockname(sockfd, (struct sockaddr*)&cliaddr, &len) < 0)
  {
    perror("getsockname error");
    exit(1);
  }

  printf("local address: %s, port: %d\n", inet_ntop(AF_INET, &cliaddr.sin_port, recvline, sizeof(recvline)), ntohs(cliaddr.sin_port));

  while ((n = read(sockfd, recvline, MAXLINE)) > 0)
  {
    recvline[n] = '\0';

    fputs(recvline, stdout);
  }

  return 0;
}