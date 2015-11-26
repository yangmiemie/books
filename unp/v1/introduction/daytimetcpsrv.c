#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>

#define MAXLINE 64
#define BACKLOG 16

int main(int argc, char const *argv[])
{
  int listenfd, connfd;
  struct sockaddr_in servaddr, clientaddr;
  char buff[MAXLINE + 1] = {0};
  time_t ticks;
  int port, len;
  pid_t pid;

  if (argc != 2)
  {
    fprintf(stderr, "usage: daytimetcpsrv <port>\n");
    exit(1);
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  port = atoi(argv[1]);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  // printf("htons(%d) = %d\n", port, htons(port));
  if (bind(listenfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind error");
    exit(1);
  }

  len = sizeof(clientaddr);
  if (getsockname(listenfd, (struct sockaddr*)&clientaddr, &len) < 0)
  {
    perror("getsockname error");
    exit(1);
  }

  printf("local address: %s, port: %d\n", inet_ntop(AF_INET, &clientaddr.sin_port, buff, sizeof(buff)), ntohs(clientaddr.sin_port));

  // if (listen(listenfd, BACKLOG) < 0)
  // {
  //   perror("listen error");
  //   exit(1);
  // }

  printf("listen = %d\n", listenfd);
  while (1)
  {
    len = sizeof(clientaddr);
    if ((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len) < 0))
    {
      perror("accept error");
      exit(1);
    }

    if ((pid = fork()) == 0)
    {
      close(listenfd);
      printf("connection from %s, port %d\n", inet_ntop(AF_INET, &clientaddr.sin_addr, buff, sizeof(buff)), ntohs(clientaddr.sin_port));
      ticks = time(NULL);
      printf("ticks = %d\n", ticks);
      memset(buff, 0, sizeof(buff));
      snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
      printf("%s\n", buff);
      write(connfd, buff, strlen(buff));
      sleep(5);
      close(connfd);
      exit(1);
    }

    close(connfd);    
  }
  return 0;
}