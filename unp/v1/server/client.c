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
#include <string.h>

#define MAXLINE 64
#define MAXN 16384

int main(int argc, char const *argv[])
{
  int i, j, pid, port, nchildren, nloops, nbytes;
  struct sockaddr_in servaddr;
  int sockfd, n;
  char request[MAXLINE], reply[MAXN];

  if (argc != 6)
  {
    fprintf(stderr, "usage : client <IPAddr> <port> <#children> <#nloops/child> <#nbytes/request>\n");
    exit(1);
  }

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr.s_addr) < 0)
  {
    perror("inet_pton error");
    exit(1);
  }

  nchildren = atoi(argv[3]);
  nloops = atoi(argv[4]);
  nbytes = atoi(argv[5]);

  memset(request, 0, MAXLINE);
  snprintf(request, sizeof(request), "%d\n", nbytes);
  printf("%s", request);
  printf("sockfd = %d\n", sockfd);

  for (i = 0; i < nchildren; ++i)
  {
    if ((pid = fork()) == 0)
    {
      for (j = 0; j < nloops; ++j)
      {
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
          perror("socket error");
          exit(1);
        }

        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        {
          perror("connect error");
          exit(1);
        }

        write(sockfd, request, strlen(request));

        if ((n = read(sockfd, reply, MAXN)) != nbytes)
        {
          fprintf(stderr, "server returned %d bytes\n", n);
          exit(1);
        }

        close(sockfd);
      }

      printf("child %d done\n", i);
      exit(1);
    }
  }

  while (wait(NULL) > 0)
    ;

  return 0;
}