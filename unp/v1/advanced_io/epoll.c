#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENTS 10
#define LISTENQ 16
#define MAXLINE 2

void setnonblocking(int fd)
{
  int val;

  val = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, val | O_NONBLOCK);
}

void do_use_fd(int fd)
{
  char recvline[MAXLINE + 1];
  int n;

  printf("date coming\n");
  if ((n = read(fd, recvline, MAXLINE)) > 0)
  {
    recvline[n] = '\0';
    write(1, recvline, n);
  }

  if (n == 0)
  {
    printf("End of Socket\n");
  }
  else if (n < 0 )
  {
    if (errno != EWOULDBLOCK)
    {
      perror("read error");
      exit(1);
    }
  }
}

int main(int argc, char const *argv[])
{
  struct epoll_event ev, events[MAX_EVENTS];
  int listen_sock, conn_sock, nfds, epollfd;
  struct sockaddr_in cliaddr, servaddr;
  int len, n;

  if (argc != 2)
  {
    fprintf(stderr, "usage: srvselect <port>\n");
    exit(1);
  }

  if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket error");
    exit(1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);

  if (bind(listen_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("bind error");
    exit(1);
  }

  if (listen(listen_sock, LISTENQ) < 0)
  {
    perror("listen error");
    exit(1);
  }

  epollfd = epoll_create(10);
  if (epollfd == -1) {
      perror("epoll_create");
      exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN;
  ev.data.fd = listen_sock;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
      perror("epoll_ctl: listen_sock");
      exit(EXIT_FAILURE);
  }

  for (;;) {
     nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
     if (nfds == -1) {
         perror("epoll_pwait");
         exit(EXIT_FAILURE);
     }

     for (n = 0; n < nfds; ++n) {
         if (events[n].data.fd == listen_sock) {
             conn_sock = accept(listen_sock, (struct sockaddr *) &cliaddr, &len);
             if (conn_sock == -1) {
                 perror("accept");
                 exit(EXIT_FAILURE);
             }
             setnonblocking(conn_sock);
             ev.events = EPOLLIN;
             ev.data.fd = conn_sock;
             if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                 perror("epoll_ctl: conn_sock");
                 exit(EXIT_FAILURE);
             }
         } 
         else {
            do_use_fd(events[n].data.fd);
         }
     }
  }
  return 0;
}


// /* Set up listening socket, 'listen_sock' (socket(),
//   bind(), listen()) */

// epollfd = epoll_create(10);
// if (epollfd == -1) {
//    perror("epoll_create");
//    exit(EXIT_FAILURE);
// }

// ev.events = EPOLLIN;
// ev.data.fd = listen_sock;
// if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
//    perror("epoll_ctl: listen_sock");
//    exit(EXIT_FAILURE);
// }

// for (;;) {
//    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
//    if (nfds == -1) {
//        perror("epoll_pwait");
//        exit(EXIT_FAILURE);
//    }

//    for (n = 0; n < nfds; ++n) {
//        if (events[n].data.fd == listen_sock) {
//            conn_sock = accept(listen_sock, (struct sockaddr *) &local, &addrlen);
//            if (conn_sock == -1) {
//                perror("accept");
//                exit(EXIT_FAILURE);
//            }
//            setnonblocking(conn_sock);
//            ev.events = EPOLLIN | EPOLLET;
//            ev.data.fd = conn_sock;
//            if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
//                        &ev) == -1) {
//                perror("epoll_ctl: conn_sock");
//                exit(EXIT_FAILURE);
//            }
//        } 
//        else {
//            do_use_fd(events[n].data.fd);
//        }
//    }
// }
