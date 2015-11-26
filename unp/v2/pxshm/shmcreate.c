#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define FILE_MODE 0666

extern int optind, opterr, optopt;
extern int errno;

int main(int argc, char *argv[])
{
  int c, flag, fd;

  flag = O_CREAT | O_RDWR;
  while ((c = getopt(argc, argv, "e")) != -1)
  {
    switch(c)
    {
      case 'e':
        flag |= O_EXCL;
      break;
    }
  }

  if (optind != argc - 2)
  {
    fprintf(stderr, "usage: shmcreate [ -e ] <name> <length>\n");
    exit(1);
  }

  printf("File: %s\n", argv[optind]);
  if ((fd = shm_open(argv[optind], flag, FILE_MODE)) < 0)
  {
    perror("shm_open error");
    exit(1);
  }

  printf("Size: %d\n", atoi(argv[optind + 1]));
  if (ftruncate(fd, atoi(argv[optind + 1])) < 0)
  {
    perror("ftruncate error");
    printf("errno = %d\n", errno);
    exit(1);
  }

  return 0;
}