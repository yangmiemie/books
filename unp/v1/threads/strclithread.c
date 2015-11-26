#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 64

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void* copyto(void *arg)
{
  char sendline[MAXLINE];
  int n, sockfd;

  sockfd = *((int*)arg);

  while ((n = read(0, sendline, MAXLINE)) > 0)
  {
    sendline[n] = '\0';
    write(sockfd, sendline, n);
  }

  printf("copyto n = %d\n", n);
  shutdown(sockfd, SHUT_WR);
}

void str_cli(int inputfd, int sockfd)
{
  char readline[MAXLINE];
  int n;
  pthread_t tid;

  pthread_create(&tid, NULL, copyto, &sockfd);

  while ((n = read(sockfd, readline, MAXLINE)) > 0)
  {
    readline[n] = '\0';
    write(1, readline, n);
  }

  printf("str_cli n = %d\n", n);
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in servaddr;
  int sockfd, infd;
  int fd[5], i;

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

  str_cli(infd, sockfd);

  return 0;
}