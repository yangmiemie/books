#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

#define MAXLINE 64

void dg_cli(int sockfd, struct sockaddr_in* servcaddr, int servlen)
{
  char recvline[MAXLINE], sendline[MAXLINE], addr[MAXLINE];
  int n, port;

  if (inet_ntop(AF_INET, &servcaddr -> sin_addr.s_addr, addr, MAXLINE) == NULL)
  {
    perror("inet_ntop error");
    exit(1);
  }

  port = ntohs(servcaddr -> sin_port);

  if (connect(sockfd, (struct sockaddr*)servcaddr, servlen) < 0)
  {
    perror("connect error");
    exit(1);
  }

  while (fgets(recvline, MAXLINE, stdin) != NULL)
  {
    if ((n = write(sockfd, recvline, strlen(recvline))) < 0)
    {
      perror("sendto error");
      exit(1);
    }

    if ((n = read(sockfd, recvline, MAXLINE)) < 0)
    {
      perror("recvfrom error");
    }

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