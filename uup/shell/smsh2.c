#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 2
#define is_delim(x) ((x) == ' ' || (x) == '\t')

char* new_string(char* s, int len);
void freelist(char** list);
char** split_line(char* cmd);
char* next_comand(FILE *fp);
int execute(char** arglist);
void err_ret(char* s1, char* s2);
int process(char** args);
int do_command_control(char** args);
int ok_to_execute();
int is_command_control(char *cmd);

enum states { NEUTRAL, NEED_THEN, THEN_BLOCK };
enum results { SUCCESS, FAIL };
int if_state = NEUTRAL;
int if_result = SUCCESS;

int main(int argc, char const *argv[])
{
  char *cmd, **arglist;

  while((cmd = next_comand(stdin)) != NULL)
  {
    arglist = split_line(cmd);
    process(arglist);
    freelist(arglist);
  }

  free(cmd);
  return 0;
}

int process(char** args)
{
  int rv;

  if (args == NULL)
    rv = 0;
  else if (is_command_control(args[0]))
    rv = do_command_control(args);
  else if (ok_to_execute())
    rv = execute(args);

  return rv;
}

int do_command_control(char** args)
{
  int rv, result;
  if (strcmp(args[0], "if") == 0)
  {
    if (if_state != NEUTRAL)
      err_ret("if error", "");

    result = process(args + 1);
    if_result = ((result == 0) ? SUCCESS : FAIL);
    if_state = NEED_THEN;
    rv = 0;
  }
  else if (strcmp(args[0], "then") == 0)
  {
    if (if_state != NEED_THEN)
      err_ret("then error", "");

    if_state = THEN_BLOCK;
    rv = 0;
  }
  else if (strcmp(args[0], "fi") == 0)
  {
    if (if_state != THEN_BLOCK)
      err_ret("fi error", "");

    if_state = NEUTRAL;
    rv = 0;
  }

  return rv;
}

int ok_to_execute()
{
  int rv = 1;

  if (if_state == NEED_THEN)
    rv = 0;

  if (if_state == THEN_BLOCK && if_result == SUCCESS)
    rv = 1;
  else if (if_state == THEN_BLOCK && if_result == FAIL)
    rv = 0;

  return rv;
}

int is_command_control(char *cmd)
{
  return strcmp(cmd, "if") == 0 || strcmp(cmd, "then") == 0 || strcmp(cmd, "fi") == 0;
}


void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error %s ", s1);
  perror(s2);
  exit(1);
}

int execute(char** arglist)
{
  int child_status;
  int pid;

  if ((pid = fork()) == -1)
    err_ret("fork", "");

  if (pid == 0)
  {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    execvp(arglist[0], arglist);
  }

  wait(&child_status);
  return child_status;
}

char* next_comand(FILE *fp)
{
  int c, bufspace, pos;
  char *buf;

  pos = 0;
  bufspace = 0;

  printf("%s ", ">");

  while((c = getc(fp)) != EOF)
  {
    if (pos + 1 > bufspace)
    {
      if (bufspace == 0)
      {
        buf = malloc(BUFSIZE);
        bufspace = BUFSIZE;
      }
      else
      {
        buf = realloc(buf, bufspace + BUFSIZE);
        bufspace += BUFSIZE;        
      }
    }

    if (c == '\n')
      break;

    buf[pos++] = c;
  }

  if (pos == 0 && c == EOF)
    return NULL;

  buf[pos] = '\0';
  return buf;
}

char** split_line(char* cmd)
{
  char** args;
  char *cp;
  int bufspace, argnum, len;
  char *start;

  cp = cmd;
  argnum = 0;
  bufspace = 0;
  while(*cp != '\0')
  {
    while(is_delim((*cp)))
      ++cp;

    if (argnum + 1 > bufspace)
    {
      if (argnum == 0)
        args = malloc(BUFSIZE * sizeof(char*));
      else
      {
        args = realloc(args, (BUFSIZE + bufspace) * sizeof(char*));
        bufspace += BUFSIZE;
      }
    }

    start = cp;
    while((*cp != '\0') && !is_delim(*cp))
      cp++;

    len = cp - start;
    args[argnum++] = new_string(start, len);
  }

  args[argnum] = NULL;
  return args;
}

char* new_string(char* s, int len)
{
  char *ptr = malloc(len + 1);
  strncpy(ptr, s, len);
  ptr[len] = '\0';
  return ptr;
}

void freelist(char** list)
{
  char** ptr = list;
  while(*ptr)
  {
    free(*ptr++);
  }

  free(list);
}