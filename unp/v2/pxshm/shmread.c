#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define FILE_MODE 0666

extern int optind, opterr, optopt;

int main(int argc, char *argv[])
{
  int fd, i;
  struct stat buf;
  char *ptr;
  unsigned char c;

  if (argc != 2)
  {
    fprintf(stderr, "usage: shmwrite <name>\n");
    exit(1);
  }

  if ((fd = shm_open(argv[1], O_RDWR, FILE_MODE)) < 0)
  {
    perror("shm_open error");
    exit(1);
  }

  if (fstat(fd, &buf) < 0)
  {
    perror("fstat error");
    exit(1);
  }

  printf("Size = %d\n", buf.st_size);

  ptr = mmap(NULL, buf.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

  for (i = 0; i < buf.st_size; ++i)
    if ((c = *ptr++) != i % 256)
    {
      fprintf(stderr, "ptr[%d] = %d\n", i, c);
      exit(1);
    }
    else
    {
      printf("ptr[%d] = %d\n", i, c);
    }
}