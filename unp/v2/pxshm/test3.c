#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define FILE_MODE 0666

int main(int argc, char const *argv[])
{
  char *ptr1, *ptr2;
  int fd1, fd2;
  int status;
  struct stat buf;

  if (argc != 2)
  {
    fprintf(stderr, "usage: test3 <name>\n");
    exit(1);
  }

  if ((fd1 = shm_open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0)
  {
    perror("shm_open fd1 error");
    exit(1);
  }

  ftruncate(fd1, sizeof(int));

  if ((fd2 = shm_open("motd", O_RDWR | O_CREAT, FILE_MODE)) < 0)
  {
    perror("shm_open fd2 error");
    exit(1);
  }

  fstat(fd2, &buf);
  ftruncate(fd2, buf.st_size);

  if (fork() > 0)
  {
    ptr1 = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd1, 0);
    ptr2 = mmap(NULL, buf.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd2, 0);
      
    printf("parent: ptr1 = %p, ptr2 = %p\n", ptr1, ptr2);
    sleep(5);

    printf("shared memory integer = %d\n", *ptr1);
    wait(&status);
  }
  else
  {
    ptr1 = mmap(NULL, buf.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd2, 0);
    ptr2 = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd1, 0);
      
    printf("child: ptr1 = %p, ptr2 = %p\n", ptr1, ptr2);

    *ptr2 = 100;
    exit(1);    
  }
  return 0;
}