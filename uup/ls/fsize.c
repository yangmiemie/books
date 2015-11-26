#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 256

void fsize(char* pathname);

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int main(int argc, char *argv[])
{
  fsize(argv[1]);
  return 0;
}

void fsize(char* pathname)
{
  DIR* pdir;
  struct dirent *pDirent;
  char buf[BUFSIZE];

  if ((pdir = opendir(pathname)) == NULL)
  {
    err_ret("Cannot open directory", pathname);
  }

  while((pDirent = readdir(pdir)) != NULL)
  {
    if (strcmp(pDirent->d_name, ".") == 0 
      || strcmp(pDirent->d_name, "..") == 0)
      continue;

    if (DT_DIR == pDirent->d_type)
    {
      sprintf(buf, "%s/%s", pathname, pDirent->d_name);
      buf[strlen(pathname) + strlen(pDirent->d_name) + 1] = '\0';
      fsize(buf);
    }

    printf("%s/%s\n", pathname, pDirent->d_name);
  }

  closedir(pdir);
}