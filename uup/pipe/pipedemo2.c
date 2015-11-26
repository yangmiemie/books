#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MSG1 "I want a cookie\n"
#define MSG2 "testing...\n"
#define BUFSIZE 64

void err_ret(char* s1, char* s2);

int main(int argc, char const *argv[])
{
  int pipefd[2];
  int pid, len, read_len;
  char buf[BUFSIZE];

  if (pipe(pipefd) == -1)
    err_ret("pipe error", "");

  if ((pid = fork()) == -1)
    err_ret("fork error", "");
  else if (pid == 0)
  {
    len = strlen(MSG1);
    while(1)
    {
      write(pipefd[1], MSG1, len);
      sleep(5);
    }
  } 
  else 
  {
    len = strlen(MSG2);
    while(1)
    {
      sleep(1);
      write(pipefd[1], MSG2, len);

      read_len = read(pipefd[0], buf, BUFSIZE);
      write(1, buf, read_len);
    }

  }

  return 0;
}


void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s\n", s1);
  perror(s2);
  exit(1);  
}