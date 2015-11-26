#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 64
void err_ret(char* s1, char* s2);

int main(int argc, char const *argv[])
{
  int apipe[2], len;
  char buf[BUFSIZE];

  if (pipe(apipe) == -1)
    err_ret("pipe error", "");

  printf("Got a pipe. It is descriptors: %d, %d\n", apipe[0], apipe[1]);

  if ((fgets(buf, BUFSIZE, stdin)) != NULL)
  {
    write(apipe[1], buf, strlen(buf));
  }

  len = read(apipe[0], buf, BUFSIZE);
  write(1, buf, len);

  return 0;
}

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s\n", s1);
  perror(s2);
  exit(1);  
}