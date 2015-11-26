#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#define BUFSIZE 256
#define ISUPPERCASE(x) ((((x) >= 'A') && ( (x) <= 'Z')) ? 1 : 0)

void do_ll(char* pathname);
// void swap(char*, char*);
void swap(char**, char**);
void q_sort(char** buf, int start, int end, int func(const char*, const char*));
void show_file_info(char* pathname, char* filename);
void show_dir_info(char* dirname, char** filename, int fileNum);
char* gid_to_name(gid_t gid);
char* uid_to_name(uid_t uid);
void mode_to_letters(int mode, char* pMode);
char type_to_letter(int mode);
int cmp(const char* s1,const char* s2);

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

static int reverse = 0;
static int recursive = 0;

int main(int argc, char *argv[])
{
  int c;
  while(--argc > 0 && **++argv == '-')
  {
    while((c = *++*argv) != '\0')
    {
      switch(c)
      {
        case 'r':
          reverse = 1;
          break;
        case 'R':
          recursive = 1;
          break;
      }
    }
  }

  do_ll(*argv);

  return 0;
}

void do_ll(char* pathname)
{
  DIR* pdir;
  struct dirent *pDirent;
  char* pBuf[BUFSIZE];
  int fileNum, fileLen, i;

  if ((pdir = opendir(pathname)) == NULL)
  {
    show_file_info(pathname, pathname);
    return;
    err_ret("Cannot open directory", pathname);
  }

  fileNum = 0;
  while((pDirent = readdir(pdir)) != NULL)
  {
    if (strcmp(pDirent->d_name, ".") == 0 
      || strcmp(pDirent->d_name, "..") == 0)
      continue;

    fileLen = strlen(pDirent->d_name);
    pBuf[fileNum] = malloc(fileLen + 1);
    strcpy(pBuf[fileNum], pDirent->d_name);
    pBuf[fileNum][fileLen] = '\0';

    ++fileNum;
  }
  closedir(pdir);

  q_sort(pBuf, 0, fileNum - 1, cmp);

  show_dir_info(pathname, pBuf, fileNum);

  for (i = 0; i < fileNum; ++i)
  {
    free(pBuf[i]);
  }
}

void show_dir_info(char* dirname, char** filename, int fileNum)
{
  int pathLen, i;
  char *pathname;

  for (i = 0; i < fileNum; ++i)
  {
    if (filename[i][0] == '.')
    {
      continue;
    }

    pathLen = strlen(dirname) + strlen(filename[i]) + 1;
    pathname = malloc(pathLen + 1);
    sprintf(pathname, "%s/%s", dirname, filename[i]);
    pathname[pathLen] = '\0';

    show_file_info(pathname, filename[i]);

    free(pathname);
    pathname = NULL;
  }
}

void show_file_info(char* pathname, char* filename)
{
  struct stat sb;
  char mode[10] = "---------";

  if (lstat(pathname, &sb) == -1)
  {
    err_ret("stat", pathname);
  }

  putchar(type_to_letter(sb.st_mode));
  
  mode_to_letters(sb.st_mode, mode);
  printf("%s.", mode);

  printf("%2.d", sb.st_nlink);

  printf(" %-10s", uid_to_name(sb.st_uid));
  printf(" %-10s", gid_to_name(sb.st_gid));

  printf("%8d", sb.st_size);
  
  printf(" %.12s", ctime(&(sb.st_mtime)) + 4);
  printf(" %s\n", filename);
}

char* uid_to_name(uid_t uid)
{
  struct passwd* pPwd;

  if ((pPwd = getpwuid(uid)) == NULL)
  {
    err_ret("getpwuid", "");
  }

  return pPwd->pw_name;
}

char* gid_to_name(gid_t gid)
{
  struct group* pGrp;

  if ((pGrp = getgrgid(gid)) == NULL)
  {
    err_ret("getgrgid", "");
  }

  return pGrp->gr_name;
}

void mode_to_letters(int mode, char* pMode)
{
  if (mode & S_IRUSR) pMode[0] = 'r';
  if (mode & S_IWUSR) pMode[1] = 'w';
  if (mode & S_IXUSR) pMode[2] = 'x';
  if (mode & S_ISUID) pMode[2] = 's';
  if (mode & S_IRGRP) pMode[3] = 'r';
  if (mode & S_IWGRP) pMode[4] = 'w';
  if (mode & S_IXGRP) pMode[5] = 'x';
  if (mode & S_ISGID) pMode[5] = 'x';
  if (mode & S_IROTH) pMode[6] = 'r';
  if (mode & S_IWOTH) pMode[7] = 'w';
  if (mode & S_IXOTH) pMode[8] = 'x';
}

char type_to_letter(int mode)
{
  char c;

  switch(mode & S_IFMT)
  {
    case S_IFBLK:   c = 'b';    break;
    case S_IFCHR:   c = 'c';    break;
    case S_IFDIR:   c = 'd';    break;
    case S_IFIFO:   c = 'f';    break;
    case S_IFLNK:   c = 'l';    break;
    case S_IFREG:   c = '-';    break;
    case S_IFSOCK:  c = 's';    break;
    default:        c = 'u';    break;
  }

  return c;
}

void q_sort(char** buf, int start, int end, int func(const char*, const char*))
{
  int first, last, pivot;
  int reverseFlag = 1;

  if (start >= end)
  {
    return;
  }

  first = start;
  last = start + 1;
  pivot = start;

  while(last <= end)
  {
    if (reverse > 0)
    {
      reverseFlag = -1;
    }

    if (reverseFlag * func(buf[pivot], buf[last]) > 0)
    {
      swap(&buf[++first], &buf[last]);
    }

    ++last;
  }

  swap(&buf[first], &buf[pivot]);

  q_sort(buf, start, first - 1, func);
  q_sort(buf, first + 1, end, func);
}

int cmp(const char* s1, const char* s2)
{
  while((ISUPPERCASE(*s1) ? *s1 + 'a' - 'A' : *s1) == (ISUPPERCASE(*s2) ? *s2 + 'a' - 'A' : *s2))
  {
    if (*s1 == '\0')
    {
      return 0;
    }

    ++s1;
    ++s2;
  } 

  return (ISUPPERCASE(*s1) ? *s1 + 'a' - 'A' : *s1) - (ISUPPERCASE(*s2) ? *s2 + 'a' - 'A' : *s2);
}
// void q_sort(char* buf, int start, int end, int func(const char, const char))
// {
//  int first, last, pivot;

//  if (start >= end)
//  {
//    return;
//  }

//  first = start;
//  last = start + 1;
//  pivot = start;

//  while(last <= end)
//  {
//    if (func(buf[pivot], buf[last]) > 0)
//    {
//      swap(&buf[++first], &buf[last]);
//    }

//    ++last;
//  }

//  swap(&buf[first], &buf[pivot]);

//  q_sort(buf, start, first - 1, func);
//  q_sort(buf, first + 1, end, func);
// }


void swap(char** s1, char** s2)
{
  char* tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}

// void swap(char* s1, char* s2)
// {
//  char* tmp = s1;
//  s1 = s2;
//  s2 = tmp;
// }