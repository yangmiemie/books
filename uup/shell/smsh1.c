#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 2
#define DEL_PROMPT ">"
#define MAXVARS 200
#define MAXCMDS 2
#define MAXEXECCMDS 64

void err_ret(char* s1, char* s2);
void setup();
int execute(char *argv[]);
char *next_command(char* prompt, FILE *fp);
char** splitline(char* line);
void freelist(char** args);
char* new_string(char* s, int len);
int process(char** args);
int is_control_command(char *cmd);
int ok_to_execute(char** args);
int do_control_command(char **args);
char** split_command(char* cmdline);
int is_numeric(char *s);
void push_to_exec_cmd(char**);

enum states { NEUTRAL, WANT_THEN, THEN_BLOCK, ELSE_BLOCK };
enum results { SUCCESS, FAIL };

int if_state = NEUTRAL;
int if_result = SUCCESS;
int last_result = SUCCESS;

char*** exec_cmds;
char*** exec_cmd_ptr;

int exec_number;

struct var
{
  char *str;
  int global;
};

struct var tab[MAXVARS];

char* new_var_string(char*, char*);
struct var *find_item(char*, int);
int builtin_command(char **args);
int VLstore(char* name, char* value);
int assign(char* str);
int okname(char* str);
int VLexport(char* name);
void VLlist();

int VLenviron2table(char** env);

extern char** environ;

int main(int argc, char *argv[])
{
  char *cmdline, **arglist;
  char *prompt;
  char **single_cmds;
  int i, cmd_number;
  int rv;

  prompt = DEL_PROMPT;
  setup();

  exec_cmds = malloc(sizeof(char*) * MAXEXECCMDS);

  while((cmdline = next_command(prompt, stdin)) != NULL)
  {
    single_cmds = split_command(cmdline);

    for (i = 0; single_cmds[i] != '\0'; ++i)
    {
      if ((arglist = splitline(single_cmds[i])) != NULL)
      {
        // for those arglist executed later, we don't free it.
        // we free later arglists in do_control_command's fi.
        if ((rv = process(arglist)) != -1)
          freelist(arglist);
      }
    }

    freelist(single_cmds);
    free(cmdline);
  }

  free(exec_cmds);
  return 0;
}

char** split_command(char* cmdline)
{
  char *ptr, cmd;
  int i, len, bufspace;

  ptr = cmdline;
  i = 0;
  char** single_cmds = malloc(sizeof(char*) * MAXCMDS);
  bufspace = MAXCMDS;

  while(1)
  {
    if (i + 1 >= bufspace)
    {
      single_cmds = realloc(single_cmds, (bufspace + MAXCMDS) * sizeof(char*));
      bufspace += MAXCMDS;
    } 

    if ((ptr = strchr(cmdline, ';')) != NULL)
      *ptr = '\0';

    single_cmds[i] = new_string(cmdline, strlen(cmdline));
    ++i;

    if (ptr != NULL)
    {
      *ptr = ';';
      cmdline = ptr + 1;
    }
    else
      break;
  }

  single_cmds[i] = NULL;

  return single_cmds;
}

int VLenviron2table(char** env)
{
  char *ptr;
  int i, len;

  for (i = 0; env[i] != NULL; ++i)
  {
    if (i == MAXVARS)
      return 0;

    len = strlen(env[i]);
    ptr = malloc(len + 1);
    strcpy(ptr, env[i]);
    ptr[len] = '\0';

    tab[i].str = ptr;
    tab[i].global = 1;
  }

  return 1;
}

int builtin_command(char **args)
{
  int rv = 0;
  int exit_val;

  if (strcmp(args[0], "set") == 0)
  {
    VLlist();
    rv = 1;
  }
  else if (strchr(args[0], '=') != NULL)
  {
    rv = assign(args[0]);
  }
  else if (strcmp(args[0], "export") == 0)
  {
    if ((args[1] != NULL) && okname(args[1]))
      VLexport(args[1]);

    rv = 1;
  }
  else if (strcmp(args[0], "exit") == 0)
  {
    exit_val = 0;
    if (args[1] == NULL || is_numeric(args[1]))
    {
      if (args[1] != NULL)
        exit_val = atoi(args[1]);

      exit(exit_val);
    }
  }

  return rv;
}

int is_numeric(char *s)
{
  char *ptr;
  for (ptr = s; *ptr != '\0'; ++ptr)
    isdigit(*ptr);

  return ptr != s;
}

int assign(char* str)
{
  char *ptr;
  int rv;

  ptr  = strchr(str, '=');
  *ptr = '\0';

  if (okname(str))
    rv = VLstore(str, ptr + 1);

  *ptr = '=';
  return rv;
}

char* new_var_string(char* name, char* value)
{
  char* retval;
  retval = malloc(strlen(name) + strlen(value) + 2);
  if (retval != NULL)
    sprintf(retval, "%s=%s", name, value);
  retval[strlen(name) + strlen(value) + 1] = '\0';

  return retval;  
}

int okname(char* str)
{
  char *ptr;
  for (ptr = str; *ptr != '\0'; ++ptr)
  {
    if ((isdigit(*ptr) && (ptr == str)) || !isalnum(*ptr))
      return 0;
  }

  return (ptr != str);
}

struct var *find_item(char* name, int first_blank)
{
  int i;
  int len = strlen(name);

  for (i = 0; (i < MAXVARS) && (tab[i].str != NULL); ++i)
  {
    if ((strncmp(tab[i].str, name, len) == 0) && (tab[i].str)[len] == '=')
      return &tab[i];
  }

  if (i < MAXVARS && first_blank)
    return &tab[i];

  return NULL;
}

int VLstore(char* name, char* value)
{
  char* str;
  struct var *pvar;
  int rv = 1;

  if (((pvar = find_item(name, 1)) != NULL) && ((str = new_var_string(name, value)) != NULL))
  {
    if (pvar->str != NULL)
      free(pvar->str);

    pvar->str = str;
    rv = 0;
  }

  return rv;
}

int VLexport(char* name)
{
  struct var *pvar;
  int rv = 1;

  if ((pvar = find_item(name, 0)) != NULL)
  {
    pvar->global = 1;
    rv = 0;
  }
  else if (VLstore(name, ""))
  {
    rv = VLexport(name);
  }

  return rv;
}

void VLlist()
{
  int i;

  for (i = 0; (i < MAXVARS) && (tab[i].str != NULL); ++i)
  {
    if (tab[i].global)
      printf("* %s\n", tab[i].str);
    else
      printf("  %s\n", tab[i].str);
  }
}

int process(char** args)
{
  int rv;

  if (args[0] == NULL)
    return 0; 
  else if (is_control_command(args[0]))
    rv = do_control_command(args);
  else if ((rv = ok_to_execute(args)) == 1)
    if (!builtin_command(args))
      rv = execute(args);

  return rv;
}

int is_control_command(char *cmd)
{
  return strcmp(cmd, "if") == 0 || strcmp(cmd, "fi") == 0 || strcmp(cmd, "then") == 0 || strcmp(cmd, "else") == 0;
}

int ok_to_execute(char** args)
{
  int rv = 1;

  if (if_state == WANT_THEN)
    return 0;

  if (if_state == THEN_BLOCK && if_result == SUCCESS)
  {
    push_to_exec_cmd(args);
    rv = -1;
  }
  else if (if_state == THEN_BLOCK && if_result == FAIL)
    rv = 0;

  if (if_state == ELSE_BLOCK && if_result == FAIL)
  {
    push_to_exec_cmd(args);
    rv = -1;    
  }
  else if (if_state == ELSE_BLOCK && if_result == SUCCESS)
    rv = 0;

  return rv;
}

void push_to_exec_cmd(char** args)
{
  exec_cmds[exec_number++] = args; 
}

int do_control_command(char** args)
{
  int rv;

  if (strcmp(args[0], "if") == 0)
  {
    if (if_state != NEUTRAL)
      err_ret("if error", "");

    last_result = process(args + 1);
    if_result = ((last_result == 0) ? SUCCESS : FAIL);
    if_state = WANT_THEN;
    rv = 0;
    printf("in do_control_command if, last_result = %d, if_result = %d, if_state = %d, arg + 1 = %s\n", last_result, if_result, if_state, *(args + 1));
  }
  else if (strcmp(args[0], "then") == 0)
  {
    if (if_state != WANT_THEN)
      err_ret("then error", "");

    if_state = THEN_BLOCK;
    rv = 0;
  }
  else if (strcmp(args[0], "else") == 0)
  {
    if (if_state != THEN_BLOCK)
      err_ret("else error", "");

    if_state = ELSE_BLOCK;
    rv = 0;
  }
  else if (strcmp(args[0], "fi") == 0)
  {
    if ((if_state != THEN_BLOCK) && (if_state != ELSE_BLOCK))
      err_ret("fi error", "");

    exec_cmd_ptr = exec_cmds;
    while(exec_number != 0)
    {
      if (!builtin_command(*exec_cmd_ptr))
        execute(*exec_cmd_ptr);

      freelist(*exec_cmd_ptr);

      ++exec_cmd_ptr;
      --exec_number;
    }

    if_state = NEUTRAL;
  }

  return rv;
}

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(0);
}

void setup()
{
  VLenviron2table(environ);
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

int execute(char *argv[])
{
  int pid, child_status;

  if ((pid = fork()) == -1)
    perror("Fork");

  if (pid == 0)
  {
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    execvp(argv[0], argv);
  }

  wait(&child_status);

  return child_status;
}

char *next_command(char* prompt, FILE *fp)
{
  int c, pos;
  int bufspace = 0;
  char *buf;

  printf("%s ", prompt);

  pos = 0;
  while((c = getc(fp)) != EOF)
  {
    if (bufspace <= pos + 1)
    {
      if (pos == 0)
        buf = malloc(BUFSIZE * sizeof(char));
      else
      {
        buf = realloc(buf, BUFSIZE + pos);
        bufspace += BUFSIZE;      
      }
    }

    if (c == '\n')
      break;

    buf[pos++] = c;
  }

  if (c == EOF || pos == 0)
    return NULL;

  buf[pos] = '\0';
  return buf;
}

int is_delim(x) 
{
  return x == ' ' || x == '\t'; 
}

char** splitline(char* line)
{
  char **args;
  char *cp;
  int bufspace = 0;
  int argnum;
  int len;
  char* start, end;

  if (line == NULL)
    return NULL;

  args = malloc(BUFSIZE * sizeof(char*));
  bufspace = BUFSIZE;
  argnum = 0;
  cp = line;

  while(*cp != '\0')
  {
    while(is_delim(* cp))
      cp++;

    if (argnum + 1 >= bufspace)
    {
      args = realloc(args, (bufspace + BUFSIZE) * sizeof(char*));
      bufspace += BUFSIZE;
    }

    start = cp;
    len = 1;
    while(*++cp != '\0' && !(is_delim(*cp)))
      len++;
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

void freelist(char** args)
{
  char **ptr = args;

  while(*args)
  {
    free(*args);
    ++args;
  }

  free(ptr);
}
