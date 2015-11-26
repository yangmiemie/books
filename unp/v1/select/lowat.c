#include <sys/select.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 64
#define LISTENQ 16

int main(int argc, char const *argv[])
{
  int maxfd, sockfd, connfd;
  fd_set rset, allset, wset;
  int nready, n;
  char recvline[MAXLEN];
  struct sockaddr_in cliaddr, servaddr;
  int len, bufsize;

  if (argc != 2)
  {
    fprintf(stderr, "usage: srvselect <port>\n");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

  if (listen(sockfd, LISTENQ) < 0)
  {
    perror("listen error");
    exit(1);
  }

  len = sizeof(cliaddr);
  connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

  printf("connection established, connfd = %d\n", connfd);

  len = sizeof(bufsize);
  if (getsockopt(connfd, SOL_SOCKET, SO_SNDLOWAT, &bufsize, &len) < 0)
  {
    perror("getsockopt error");
    exit(1);
  }

  printf("SO_SNDLOWAT = %d\n", bufsize);

  bufsize = 4;

  if (setsockopt(connfd, SOL_SOCKET, SO_SNDLOWAT, &bufsize, sizeof(bufsize)) < 0)
  {
    perror("setsockopt error");
    exit(1);
  }

  if (getsockopt(connfd, SOL_SOCKET, SO_SNDLOWAT, &bufsize, &len) < 0)
  {
    perror("getsockopt error");
    exit(1);
  }

  printf("after setsockopt, SO_SNDLOWAT = %d\n", bufsize);

  maxfd = connfd;

  FD_ZERO(&allset);
  FD_SET(connfd, &allset);

  char* snd = "0";
  printf("send string: %s\n", snd);
  for ( ; ; )
  {
    rset = allset;
    wset = allset;
    nready = select(maxfd + 1, &rset, &wset, NULL, NULL);

    if (FD_ISSET(connfd, &rset))
    {
      if ((n = read(connfd, recvline, MAXLEN)) > 0)
      {
        recvline[n] = '\0';
        printf("from stdin: %s", recvline);
      }
      else if (n == 0)
      {
        printf("connection end\n");
      }
    }

    if (FD_ISSET(connfd, &wset))
    {
      sleep(2);
      write(connfd, snd, strlen(snd));
    }

  }
  return 0;
}