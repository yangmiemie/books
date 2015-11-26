#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char* args[]);
char *make_string(char* buf);

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
    }
  }

  return 0;
}

void execute(char* args[])
{
  char* cmd = args[0];
  execvp(cmd, args);
  exit(1);
}

char *make_string(char* buf)
{
  char *ptr;
  ptr = malloc(sizeof(char) * (strlen(buf)));
  strcpy(ptr, buf);
  ptr[strlen(buf) - 1] = '\0';
  return ptr;
}