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

#define MAXLINE 64

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int connect_nonb(int sockfd, struct sockaddr* servaddr, int len, int nsec)
{
  int flags;
  int n, maxfd, errlen, error;
  fd_set rset, wset;
  struct timeval tv;

  flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  printf("connect timeout time: %d\n", nsec);

  if ((n = connect(sockfd, servaddr, len)) < 0)
    if (errno != EINPROGRESS)
      return -1;

  if (n == 0)
    goto done;

  printf("n: %d\n", n);


  FD_ZERO(&rset);
  FD_ZERO(&wset);

  FD_SET(sockfd, &wset);
  FD_SET(sockfd, &rset);

  tv.tv_sec = nsec;
  tv.tv_usec = 0;

  maxfd = sockfd;
  error = 0;


  if ((n = select(maxfd + 1, &rset, &wset, NULL, &tv)) == 0)
  {
    close(sockfd);
    errno = ETIMEDOUT;
    return -1; 
  }

  if (FD_ISSET(sockfd, &rset) || (FD_ISSET(sockfd, &wset)))
  {
    errlen = sizeof(error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &errlen) < 0)
    {
      return -1;
    }
  }

done:
  fcntl(sockfd, F_SETFL, flags);

  if (error)
  {
    close(sockfd);
    errno = errno;
    return -1;
  }

  return 0;
}

void str_cli(int inputfd, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  int n, maxfd;
  int writeBytes, readBytes;
  int fd;
  int stdineof;

  fd_set rset;
  readBytes = writeBytes = 0;

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
          return;
        }
        else
        {
          fprintf(stderr, "cliselect: server teminated prematurely\n");
          return;
        }
      }

      readBytes += n;
      recvline[n] = '\0';
      write(1, recvline, n);
      // printf("\n");
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

  // if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  // {
  //   perror("connect error");
  //   exit(1);
  // }

  if (connect_nonb(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr), 5) < 0)
  {
    perror("connect error");
    exit(1);
  }

  infd = 0;

  str_cli(infd, sockfd);
  return 0;
}