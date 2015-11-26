#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLINE 64
#define NUMBER 5

typedef void (*sighandler_t)(int);

void str_cli(int sockfd)
{
  char buff[MAXLINE];
  char addr[MAXLINE] = {0};
  int n, len;
  struct sockaddr_in servaddr;
  int fd;

  len = sizeof(servaddr);
  if (getpeername(sockfd, (struct sockaddr*)&servaddr, &len) < 0)
  {
    perror("getpeername error");
    exit(1);
  }

  if (inet_ntop(AF_INET, &(servaddr.sin_addr), addr, MAXLINE) < 0)
  {
    perror("inet_ntop error");
    exit(1);
  }

  if ((fd = open("tcpserv1", O_RDONLY)) < 0)
  {
    perror("open tcpserv1 error");
    exit(1);
  }

  printf("fd = %d\n", fd);

  // dup2(fd, 0);

  close(fd);

  while (fgets(buff, MAXLINE, stdin) != NULL)
  {
    // printf("fgets: %s\n", buff);
    write(sockfd, buff, strlen(buff));

    memset(buff, 0, sizeof(buff));

    if ((n = read(sockfd, buff, MAXLINE)) > 0)
    {
      buff[n] = '\0';
      printf("read from %s(%d): %s", addr, ntohs(servaddr.sin_port), buff);
    }
    else if (n == 0)
    {
      fprintf(stderr, "str_cli: server terminated permaturely\n");
      // exit(1);
    }
    else
    {
      fprintf(stderr, "str_cli: read error\n");
    }
  }
}

sighandler_t Signal(int signo, sighandler_t func)
{
  struct sigaction act, oact;
  sigset_t set;

  act.sa_handler = func;
  sigemptyset(&set);
  act.sa_mask = set;
  act.sa_flags = 0;
  act.sa_flags |= SA_NODEFER;  
  act.sa_flags |= SA_RESTART;

  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;

  return oact.sa_handler;
}

void sig_pipe(int signo)
{
  printf("receive SIGPIPE\n");
}

int main(int argc, char const *argv[])
{
  int i, sockfd[NUMBER];
  struct sockaddr_in servaddr;

  if (argc != 3)
  {
    fprintf(stderr, "usage: tcpcli1 <address> <port>\n");
    exit(1);
  }

  Signal(SIGPIPE, sig_pipe);
  i = 0;
  // for (i = 0; i < NUMBER; ++i)
  {
    if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("socket error");
      exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
    {
      perror("inet_pton");
      exit(1);
    }

    if (connect(sockfd[i], (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
      perror("connect error");
      exit(1);
    }

  }
  
  str_cli(sockfd[0]);

  return 0;
}