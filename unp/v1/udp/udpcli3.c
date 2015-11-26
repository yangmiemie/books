#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

#define MAXLINE 30000
#define NDG 20000

void dg_cli(int sockfd, struct sockaddr_in* servcaddr, int servlen)
{
  int i;
  char sendline[MAXLINE];

  for (i = 0; i < NDG; ++i)
    sendto(sockfd, sendline, MAXLINE, 0, (struct sockaddr*)servcaddr, servlen);
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

  memset(&servcaddr, 0, sizeof(servcaddr));
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