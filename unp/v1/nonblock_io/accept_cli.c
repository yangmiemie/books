#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>

int main(int argc, char const *argv[])
{
  struct sockaddr_in servaddr;
  int sockfd;
  struct linger ling;

  if (argc != 3)
  {
    fprintf(stderr, "usage: cliselect <ip> <port>\n");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr) < 0)
  {
    perror("inet_pton error");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("connect error");
    exit(1);
  }

  ling.l_onoff = 1;
  ling.l_linger = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
  close(sockfd);

  exit(1);
  return 0;
}