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

void str_cli(int inputfd, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  int n, maxfd;
  int writeBytes, readBytes;
  int fd;
  int stdineof;

  fd_set rset;
  readBytes = writeBytes = 0;

  if ((fd = open("leetcode.txt", O_RDONLY)) < 0)
  {
    perror("open error");
    exit(1);
  }

  // dup2(fd, 0);

  for (;;)
  {
    FD_SET(sockfd, &rset);
    FD_SET(inputfd, &rset);

    if (stdineof == 1)
      FD_CLR(inputfd, &rset);
    
    maxfd = MAX(sockfd, inputfd) + 1;
    
    select(maxfd, &rset, NULL, NULL, NULL);

    if (FD_ISSET(inputfd, &rset))
    {
      if ((n = read(inputfd, sendline, MAXLINE)) <= 0)
      {
        stdineof = 1;
        shutdown(sockfd, SHUT_WR);
        FD_CLR(inputfd, &rset);
        printf("\nreadBytes = %d, writeBytes = %d\n", readBytes, writeBytes);
        continue;
      }

      writeBytes += n;
      write(sockfd, sendline, n);
    }

    if (FD_ISSET(sockfd, &rset))
    {
      if ((n = read(sockfd, recvline, MAXLINE)) < 0)
      {
        printf("read error\n");
        return;
      }
      else if (n == 0)
      {
        if (stdineof == 1)
        {
          printf("End of Socket\n");
          printf("\nreadBytes = %d, writeBytes = %d\n", readBytes, writeBytes);          
          // return;
        }
        else
        {
          fprintf(stderr, "cliselect: server teminated prematurely\n");
          // return;
        }
      }

      readBytes += n;
      recvline[n] = '\0';
      write(1, recvline, n);
      printf("\n");
      // printf("%s", recvline);
    }
  }
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

  infd = 0;

  str_cli(infd, sockfd);


  // for (i = 0; i < 5; ++i)
  // {
  //   if ((fd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  //   {
  //     perror("socket error");
  //     exit(1);
  //   }

  //   servaddr.sin_family = AF_INET;
  //   servaddr.sin_port = htons(atoi(argv[2]));

  //   if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr) < 0)
  //   {
  //     perror("inet_pton error");
  //     exit(1);
  //   }
  //   if (connect(fd[i], (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  //   {
  //     perror("connect error");
  //     exit(1);
  //   }

  //   infd = 0;

  // }

  // str_cli(infd, fd[0]);
  return 0;
}