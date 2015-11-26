#include <stdio.h>

int main(int argc, char const *argv[])
{
  int fd;
  FILE *fp;
  int pid;
  char* msg1 = "Test 1 2 3 ..\n";
  char *msg2 = "Hello, world!\n";

  // if ((fd = creat("testfile", 0644)) == -1)
  //  return 0;

  // if (write(fd, msg1, strlen(msg1)) == -1)
  //  return 0;

  // if ((pid = fork()) == -1)
  //  return 0;

  // if (write(fd, msg2, strlen(msg2)) == -1)
  //  return 0;

  // close(fd);

  fp = fopen("testfile2", "w");

  // fp = stdout;

  fprintf(fp, "%s", msg1);
  fflush(fp);
  
  if ((pid = fork()) == -1)
    return 0;
  
  fprintf(fp, "%s", msg2);
  fclose(fp);
  return 1;
}