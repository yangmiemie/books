#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  struct stat sbuf;
  int id;

  if (argc != 2)
  {
    fprintf(stderr, "usage: ftok <pathname>\n");
    exit(1);
  }

  stat(argv[1], &sbuf);
  id = 0x123;

  printf("st_dev: %lx, st_ino: %lx, id: %lx, key: %lx\n", sbuf.st_dev, sbuf.st_ino, id, ftok(argv[1], id));

  return 0;
}