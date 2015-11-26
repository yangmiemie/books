#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

extern char *optarg;
extern int optind;

#define FILE_MODE 0644

int main(int argc, char *argv[])
{
  int c, flags;
  int value;
  sem_t* sem;
  
  flags = O_RDWR | O_CREAT;
  value = 1;
  while ((c = getopt(argc, argv, "i:e")) != -1)
  {
    switch(c)
    {
      case 'i':
        value = atoi(optarg);
        break;
      case 'e':
        flags |= O_EXCL;
        break;
    }
  }

  if (optind != argc - 1)
  {
    fprintf(stderr, "usage: semcreate [ -e ] [ -i initalvalue] <name>\n");
    exit(1);
  }

  // printf("%d\n", O_CREAT);
  // printf("%d\n", O_RDWR);
  // printf("%d\n", flags);
  // printf("%d\n", value);
  // printf("%s\n", argv[optind]);
  if ((sem = sem_open(argv[optind], O_CREAT, FILE_MODE, value)) == SEM_FAILED)
  {
    perror("semcreate error");
    exit(1);
  }

  sem_close(sem);

  return 0;
}