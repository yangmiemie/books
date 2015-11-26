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
#include <errno.h>
#include <string.h>
#include <signal.h>

#define MAXLINE 64
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void str_cli(int inputfd, int sockfd)
{
  pid_t pid;
  char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
  int n;

  if ((pid = fork()) == 0)
  {
    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
      recvline[n] = '\0';
      write(1, recvline, n);
    }

    kill(getppid(), SIGTERM);
    exit(1);
  }

  while ((n = read(0, sendline, MAXLINE)) > 0)
  {
    sendline[n] = '\0';
    write(sockfd, sendline, n);
  }

  pause();
}

int main(int argc, char const *argv[])
{
  struct sockaddr_in servaddr;
  int sockfd, infd;
  int i;

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

  printf("sockfd = %d\n", sockfd);

  infd = 0;

  str_cli(infd, sockfd);

  return 0;
}