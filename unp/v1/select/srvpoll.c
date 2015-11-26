#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
// #include <sys/select.h>
#include <poll.h>

#define LISTENQ 16
#define MAXLINE 4
#define MAXCONN 4
#define INFTIM -1
#define MAXSIZE 32

int main(int argc, char const *argv[])
{
  struct sockaddr_in cliaddr, servaddr;
  int sockfd, connfd, listenfd;
  int len, i, n;
  int maxfd, maxi, nready, port;
  char recvline[MAXLINE + 1];
  char addr[MAXSIZE], buff[MAXSIZE * 2];
  struct pollfd client[MAXCONN];

  if (argc != 2)
  {
    fprintf(stderr, "usage: srvpoll <port>\n");
    exit(1);
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);

  if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind error");
    exit(1);
  }

  if (listen(listenfd, LISTENQ) < 0)
  {
    perror("listen error");
    exit(1);
  }

  for (i = 1; i < MAXCONN; ++i)
    client[i].fd = -1;

  client[0].fd = listenfd;
  client[0].events = POLLRDNORM;

  maxi = 0;

  for ( ; ; )
  {
    nready = poll(client, maxi + 1, INFTIM);

    if (client[0].revents & POLLRDNORM)
    {
      len = sizeof(cliaddr);
      if ((connfd = accept(client[0].fd, (struct sockaddr*)&cliaddr, &len)) < 0)
      {
        perror("accept error");
        exit(1);
      }

      for (i = 1; i < MAXCONN; ++i)
        if (client[i].fd == -1)
          break;

      if (i >= MAXCONN)
      {
        fprintf(stderr, "error: too many clients\n");
        exit(1);
      }

      client[i].fd = connfd;
      client[i].events = POLLRDNORM;

      printf("connection established: connfd = %d, i = %d\n", connfd, i);

      if (i > maxi)
        maxi = i;

      if (--nready <= 0)
        continue;
    }

    for (i = 1; i <= maxi; ++i)
    {
      if ((sockfd = client[i].fd) < 0)
        continue;

      if (client[i].revents & (POLLRDNORM | POLLERR))
      {
        if ((n = read(sockfd, recvline, MAXLINE)) <= 0)
        {
          if (n < 0)
          {
            perror("read error");            
          }
          else
          {
            printf("End Of Socket\n");
          }

          client[i].fd = -1;
          close(sockfd);
        }
        else
        {

          len = sizeof(cliaddr);
          if (getpeername(sockfd, (struct sockaddr*)&cliaddr, &len) < 0)
          {
            perror("getpeername error");
            exit(1);
          }

          memset(addr, 0, MAXSIZE);
          if (inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, addr, MAXSIZE) == NULL)
          {
            perror("inet_ntop error");
            exit(1);
          }

          port = ntohs(cliaddr.sin_port);

          recvline[n] = '\0';
          sprintf(buff, "receive from %s(%d): %s", addr, port, recvline);
          printf("%s", buff);
          write(sockfd, recvline, n);
        }

        if (--nready <= 0)
          break;
      }
    }
  }

  return 0;
}

