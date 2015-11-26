#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MODE 0755

int parent_flag = 0;

int do_mkdir(char *dirname);
int is_dir_existed(char* dirname);

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int main(int argc, char *argv[])
{
  int c;
  while(--argc > 0 && **++argv == '-')
  {
    while((c = *++*argv) != '\0')
    {
      switch(c)
      {
        case 'p':
        parent_flag = 1;
        break;
        default:
        break;
      }
    }
  }

  do_mkdir(*argv);
  return 0;
}

int do_mkdir(char* dirname)
{
  char *dir, *pdir;

  if (mkdir(dirname, MODE) != -1)
  {
    return 0;
  }

  if (errno == ENOENT & parent_flag == 1)
  {
    dir = malloc(strlen(dirname) + 1);
    strcpy(dir, dirname);

    for (pdir = dir; *pdir == '/'; ++pdir);

    while((pdir = strchr(pdir, '/')) != NULL)
    {
      *pdir = '\0';

      // The file dir is not a existed dir, it may not existed or it existed but not a dir
      if (!is_dir_existed(dir) && (mkdir(dir, MODE) == -1))
      {
        err_ret("Create directory failed ", dir);
      }

      *pdir++ = '/';
    }

    free(dir);

    return do_mkdir(dirname);
  }
  else if (errno == EEXIST)
  {
    return 0;
  }
  else
  {
    err_ret("Create directory failed", dirname);
  }
}

int is_dir_existed(char* dirname)
{
  struct stat sbuf;

  return ((stat(dirname, &sbuf) != -1) && S_ISDIR(sbuf.st_mode));
}