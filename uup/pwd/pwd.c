#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 64

void pwd(int inode);
int get_inode(char* pathname);
int inode_to_name(int inode, char* buf, int size);

void err_ret(char* s1, char *s2)
{
  fprintf(stderr, "Error: %s", s1);
  perror(s2);
  exit(1);
}

int main(int argc, char const *argv[])
{
  pwd(get_inode("."));
  printf("\n");
  return 0;
}

void pwd(int inode)
{
  char buf[BUFSIZE];

  if (get_inode("..") != inode)
  {
    if (inode_to_name(inode, buf, BUFSIZE) == 0)
    {
      err_ret("Inode get name", "");
    }

    chdir("..");
    pwd(get_inode("."));

    printf("/%s", buf);
  }
}

int inode_to_name(int inode, char* buf, int size)
{
  DIR *dirp;
  struct dirent* direntp;
  int nameLen;

  if ((dirp = opendir("..")) == NULL)
  {
    err_ret("Cannot open directory", "..");
  }

  while((direntp = readdir(dirp)) != NULL)
  {
    if (direntp->d_ino == inode)
    {
      nameLen = strlen(direntp->d_name);
      if (nameLen < size)
      {
        strcpy(buf, direntp->d_name);
        return 1;
      }
    }
  }

  return 0;
}

int get_inode(char* pathname)
{
  struct stat sbuf;
  if (stat(pathname, &sbuf) == -1)
  {
    err_ret("Cannot open file", pathname);
  }

  return sbuf.st_ino;
}

