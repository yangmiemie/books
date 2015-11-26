#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char* args[]);
char *make_string(char* buf);
void pr_exit(int status);

int main(int argc, char const *argv[])
{
  char* arglist[MAXARGS];
  int numargs;
  char argbuf[ARGLEN];
  numargs = 0;

  while(numargs < MAXARGS)
  {
    printf("Arg[%d]?", numargs);
    if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
    {
      arglist[numargs++] = make_string(argbuf); 
    }
    else
    {
      arglist[numargs] = NULL;
      execute(arglist);
      numargs = 0;
    }
  }

  return 0;
}

void execute(char* args[])
{
  int child_status;
  int child_pid;
  child_pid = fork();

  switch(child_pid)
  {
    case -1:
      perror("fork failed");
      break;
    case 0:
      execvp(args[0], args);
      exit(1);
      break;
    default:
      wait(&child_status);
      pr_exit(child_status);
  }
}

char *make_string(char* buf)
{
  char *ptr;
  ptr = malloc(sizeof(char) * (strlen(buf)));
  strcpy(ptr, buf);
  ptr[strlen(buf) - 1] = '\0';
  return ptr;
}

void pr_exit(int status)
{
  if (WIFEXITED(status))
  {
    printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
  }
  else if (WIFSIGNALED(status))
  {
    printf("abnormal termination, signal number = %d\n", WTERMSIG(status));
  }
  else if (WIFSTOPPED(status))
  {
    printf("child stopped, signal number = %d\n", WSTOPSIG(status));
  }
}