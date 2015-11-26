#include <unistd.h>
#include <stdio.h>

// #include "comm.h"

int main(int argc, char const *argv[])
{
  int pipe1[2], pipe2[2];
  int pid;

  pipe(pipe1);
  pipe(pipe2);

  if ((pid = fork()) == -1)
  {
    fprintf(stderr, "Error in fork");
    exit(1);
  }
  else if (pid > 0)
  {
    close(pipe1[1]);
    close(pipe2[0]);
    client(pipe1[0], pipe2[1]);
  }
  else
  {
    close(pipe1[0]);
    close(pipe2[1]);
    server(pipe2[0], pipe1[1]);
  }
  return 0;
}