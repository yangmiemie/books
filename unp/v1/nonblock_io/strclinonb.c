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

#define MAXLINE 64
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void setnonblock(int fd)
{
  int val;

  val = fcntl(fd, F_GETFD, 0);
  fcntl(fd, F_SETFD, val | O_NONBLOCK);
}

void str_cli(int inputfd, int sockfd)
{
  char to[MAXLINE + 1], fr[MAXLINE + 1];
  int maxfd;
  fd_set rset, wset;
  char *toopr, *toipr, *froptr, *friptr;
  int stdinof;
  int n;

  setnonblock(STDIN_FILENO);
  setnonblock(STDOUT_FILENO);
  setnonblock(sockfd);

  memset(to, 0, MAXLINE + 1);
  memset(fr, 0, MAXLINE + 1);

  maxfd = MAX(MAX(STDOUT_FILENO, STDIN_FILENO), sockfd);
  
  toopr = toipr = to;
  friptr = froptr = fr;
  stdinof = 0;

  for ( ; ; )
  {
    FD_ZERO(&rset);
    FD_ZERO(&wset);

    if (stdinof != 1 && toipr < &to[MAXLINE])
      FD_SET(STDIN_FILENO, &rset);
    if (friptr < &fr[MAXLINE])
      FD_SET(sockfd, &rset);
    if (toipr > toopr)
      FD_SET(sockfd, &wset);
    if (friptr > froptr)
      FD_SET(STDOUT_FILENO, &wset);

    select(maxfd + 1, &rset, &wset, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &rset))
    {
      if ((n = read(STDIN_FILENO, toipr, &to[MAXLINE] - toipr)) < 0)
      {
        if (errno != EWOULDBLOCK)
        {
          perror("read STDIN_FILENO error");
          exit(1);
        }
      }
      else if (n == 0) 
      {
        stdinof = 0;

        if (toipr == toopr)
          shutdown(sockfd, SHUT_WR);
      }
      else 
      {
        fprintf(stderr, "read %d bytes from stdin\n", n);

        toipr += n;
        FD_SET(sockfd, &wset);
      }
    }

    if (FD_ISSET(sockfd, &rset))
    {
      if ((n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0)
      {
        if (errno != EWOULDBLOCK)
        {
          perror("read sockfd error");
          exit(1);
        }
      }
      else if (n == 0)
      {
        if (stdinof == 1)
        {
          printf("exit normally\n");
          return;
        }
        else
        {
          printf("str_cli: server teminated prematurely\n");
          exit(1);
        }
      }
      else
      {
        fprintf(stderr, "read %d bytes from server\n", n);
        friptr += n;
        FD_SET(STDOUT_FILENO, &wset);
      }
    }

    if (FD_ISSET(STDOUT_FILENO, &wset) && (friptr > froptr))
    {
      if ((n = write(STDOUT_FILENO, froptr, friptr - froptr)) < 0)
      {
        if (errno != EWOULDBLOCK)
        {
          perror("write STDOUT_FILENO error");
          exit(1);
        }
      }
      else if (n > 0)
      {
        fprintf(stderr, "write %d bytes to stdout\n", n);

        froptr += n;
        if (friptr == froptr)
          friptr = froptr = fr;
      }
    }

    if (FD_ISSET(sockfd, &wset) && (toipr > toopr))
    {
      if ((n = write(sockfd, toopr, toipr - toopr)) < 0)
      {
        if (errno != EWOULDBLOCK)
        {
          perror("write sockfd error");
          exit(1);
        } 
      }
      else
      {
        fprintf(stderr, "write %d bytes to server\n", n);
        toopr += n;
        if (toipr == toopr)
        {
          toopr = toipr = to;
          if (stdinof == 1)
          {
            shutdown(sockfd, SHUT_WR);
          }
        }
      }
    }
  }
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