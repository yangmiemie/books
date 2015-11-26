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
#define SCREENWIDTH 80
#define ISUPPERCASE(x) ((((x) >= 'A') && ( (x) <= 'Z')) ? 1 : 0)

void do_ls(char* pathname);
void show_file_info(char* pathname);
void swap(char**, char**);
void q_sort(char** buf, int start, int end, int func(const char*, const char*));
void show_file_info(char* pathname);
void show_dir_info(char* dirname, char** filename, int fileNum);
int cmp(const char* s1,const char* s2);
int get_Screen_Width();
int is_dir(char* pathname);

void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int recursive = 0;
int sort = 1;

int main(int argc, char *argv[])
{
  int c;

  while(--argc > 0 && **++argv == '-')
  {
    while((c = *++*argv) != '\0')
    {
      switch(c)
      {
        case 'R':
          recursive = 1;
          break;
        case 'q':
          sort = 0;
        default:
          break;
      }
    }
  }

  do_ls(*argv);

  return 0;
}

void do_ls(char* pathname)
{
  DIR* pdir;
  struct dirent *pDirent;
  // char** pBuf, **pTmp;
  int fileNum, fileLen, i, bufLen;
  char *pBuf[BUFSIZE];
  int pathLen;
  char *fullpathname;

  if ((pdir = opendir(pathname)) == NULL)
  {
    err_ret("Cannot open directory", pathname);
  }

  // bufLen = BUFSIZE;
  // pBuf = (char**)malloc(sizeof(char*) * BUFSIZE);

  fileNum = 0;
  while((pDirent = readdir(pdir)) != NULL)
  {
    if (recursive == 0)
    {
      if (strcmp(pDirent->d_name, ".") == 0 
        || strcmp(pDirent->d_name, "..") == 0
        || pDirent->d_name[0] == '.')
        continue;
    }

    // Use realloc to alloc memory, in the second time to realloc error occur
    // realloc: invalid next size 
    // if (fileNum >= bufLen)
    // {
    //  bufLen *= 2;
    //  if ((pTmp = (char**)realloc(pBuf, bufLen)) == (void*)NULL)
    //    err_ret("realloc error", "");

    //  pBuf = pTmp;
    // }

    fileLen = strlen(pDirent->d_name);
    pBuf[fileNum] = malloc(fileLen + 1);
    strcpy(pBuf[fileNum], pDirent->d_name);
    pBuf[fileNum][fileLen] = '\0';

    // printf("%d: ", fileNum);
    // for (i = 0; i <= fileNum; ++i)
    // {
    //  printf("%s,", pBuf[i]);;
    // }
    // printf("\n");
    // printf("%x,%x\n", pBuf, pTmp);
    ++fileNum;
  }
  closedir(pdir);

  if (sort > 0)
  {
    q_sort(pBuf, 0, fileNum - 1, cmp);
  }

  show_dir_info(pathname, pBuf, fileNum);

  if (recursive > 0)
  {
      printf("\n");
      for (i = 0; i < fileNum; ++i)
      {
          if (strcmp(pBuf[i], ".") == 0 
            || strcmp(pBuf[i], "..") == 0)
            continue;

          pathLen = strlen(pathname) + strlen(pBuf[i]) + 1;
          fullpathname = malloc(pathLen + 1);
          sprintf(fullpathname, "%s/%s", pathname, pBuf[i]);
          fullpathname[pathLen] = '\0';

          if (is_dir(fullpathname))
          {
            printf("%s:\n", fullpathname);
            do_ls(fullpathname);
          }

          free(fullpathname);
      }
  }

  for (i = 0; i < fileNum; ++i)
  {
    free(pBuf[i]);
  }

  // free(pBuf);
}

int is_dir(char* pathname)
{
  struct stat sb;


  if (stat(pathname, &sb) == -1)
  {
    err_ret("Cannot open file", pathname);
  }

  return S_ISDIR(sb.st_mode) ? 1 : 0;
}

void show_dir_info(char* dirname, char** filename, int fileNum)
{
  int maxLen, nameLen, colLen, colNum, rowNum, screenWidth;
  int i, j, k; 

  maxLen = 0;
  for (i = 0; i < fileNum; ++i)
  {
    nameLen = strlen(filename[i]);
    if (nameLen > maxLen)
    {
      maxLen = nameLen;
    }
  }

  screenWidth = get_Screen_Width();
  colNum = screenWidth / (maxLen + 1);
  colNum = colNum < 1 ? 1 : colNum;
  colLen = maxLen + 1;
  rowNum = fileNum / colNum;

  if (fileNum % colNum)
    ++rowNum;

  for (i = 0; i < rowNum; ++i)
  {
    for (j = 0; j < colNum; ++j)
    {
      k = j + i * colNum;

      if (k == fileNum)
        break;

      printf("%-*s", colLen, filename[k]);

      // last column print '\n'
      if ((k + 1) % colNum == 0)
      {
        printf("\n");
      }
    }
  }

  printf("\n");
}

int get_Screen_Width()
{
  return SCREENWIDTH;
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

void q_sort(char** buf, int start, int end, int func(const char*, const char*))
{
  int first, last, pivot;

  if (start >= end)
  {
    return;
  }

  first = start;
  last = start + 1;
  pivot = start;

  while(last <= end)
  {
    if (func(buf[pivot], buf[last]) > 0)
    {
      swap(&buf[++first], &buf[last]);
    }

    ++last;
  }

  swap(&buf[first], &buf[pivot]);

  q_sort(buf, start, first - 1, func);
  q_sort(buf, first + 1, end, func);
}

void swap(char** s1, char** s2)
{
  char* tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}