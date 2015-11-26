#include <stdio.h>

#define MAXLINES 1024
#define MAXLINELENS 1024
#define MAXALLOCSIZE MAXLINELENS * MAXLINES

char* linesptr[MAXLINES];
// char* linesptr[MAXLINES];
// cha* linesptr[MAXLINES];
// CHAR* linesptr[MAXLINES];

char allocbuf[MAXALLOCSIZE];
char *allocp = allocbuf;

int readlines(char* linesptr[], int nlines);
void writelines(char* linesptr[], int nlines);
int get_line(char buf[], int size);
char* alloc(int size);
int strcmp(const char* s1, const char* s2);
int strlen(const char* s);
// void qsort(char* linesptr[], int left, int right);
void qsort(char* linesptr[], int left, int right, int (*cmp)(void *, void*));
void swap(char* buf[], int i, int j);
int numcmp(const char*, const char*);
// reversed version of numcmp
int rnumcmp(const char*, const char*);
int istrcmp(const char* s1, const char* s2);

int main(int argc, char *argv[])
{
  int lines, reverse, ignore;
  char *argsptr, c;

  reverse = 0;
  ignore = 0;
  if (argc > 1)
  {
    while(--argc > 0 && argv[argc][0] == '-')
    {
      argsptr = ++*++argv;
      while((c = *(argsptr++)) != '\0')
      {
        switch(c)
        {
          case 'f':
            ignore = 1;
          break;
          case 'r':
            reverse = 1;
          break;
          default:
            printf("Invalid arguments\n");
            return;
          break;
        }
      }
    }
  }

  while((lines = readlines(linesptr, MAXLINES)) > 0)
  {
    qsort(linesptr, 0, lines - 1, (int (*)(void*, void*))(ignore ? istrcmp : strcmp));
    // qsort(linesptr, 0, lines - 1, (int (*)(void*, void*))(strcmp));
    writelines(linesptr, lines);
  }

  return 0;
}

int rnumcmp(const char *s1, const char *s2)
{
  return numcmp(s2, s1);
}


void uppercase(char *s)
{
  while(*s != '\0')
  {
    *s = ((*s >= 'a') && (*s <= 'z')) ? *s + 'A' - 'a' : *s;
    ++s;
  }
}

// ignore case cmp
int istrcmp(const char* s1, const char* s2)
{
  char buf1[MAXLINES], buf2[MAXLINES];
  strcpy(buf1, s1);
  strcpy(buf2, s2);
  uppercase(buf1);
  uppercase(buf2);

  return strcmp(buf1, buf2);
}

int strcmp(const char* s1, const char* s2)
{
  while(*s1 == *s2)
  {
    if (*s1 == '\0')
    {
      return 0;
    }

    ++s1;
    ++s2;
  }

  return *s1 - *s2;
}

int lencmp(const char* s1, const char* s2)
{
  return strlen(s1) - strlen(s2);
}

int numcmp(const char* s1, const char* s2)
{
  return strcmp(s1, s2);
}

int strlen(const char* s)
{
  int i = 0;
  while(s[i] != '\0')
    ++i;

  return i; 
}

void swap(char* buf[], int i, int j)
{
  char* tmp = buf[i];
  buf[i] = buf[j];
  buf[j] = tmp;
}

void qsort(char* linesptr[], int left, int right,int (*cmp)(void *, void*))
{
  int i, j;

  if (left >= right)
  {
    return;
  }

  swap(linesptr, left, (left + right) / 2);

  j = left + 1;
  i = left;
  for (; j <= right; ++j)
  {
    if ((*cmp)(linesptr[left], linesptr[j]) < 0)
    {
      continue;
    }
    else 
    {
      swap(linesptr, ++i, j);
    }

    // printf("in for: %s\n", linesptr);
  }

  // printf("after for : %s, i = %d\n", linesptr, i);

  swap(linesptr, left, i);

  qsort(linesptr, left, i - 1, cmp);
  qsort(linesptr, i + 1, right, cmp);
}

void writelines(char* linesptr[], int nlines)
{
  int i = 0;
  for (; i < nlines; ++i)
  {
    printf("%s", linesptr[i]);
  }
}

int readlines(char* linesptr[], int nlines)
{
  int linelen, linesnum;
  char buf[MAXLINELENS];

  linesnum = 0;
  while((linelen = get_line(buf, MAXLINELENS)) > 0)
  {
    // Add 1 to store '\0'
    if (linesnum >= nlines)
    {
      printf("Out of Memory\n");
      return -1;
    }

    linesptr[linesnum] = alloc(linelen + 1);
    strcpy(linesptr[linesnum], buf);
    ++linesnum;
  }

  return linesnum;
}


int get_line(char buf[], int size)
{
  int c, i;

  i = 0;
  while(((c = getchar()) != EOF) && (c != '\n') && (i < size))
  {
    buf[i++] = c;
  }

  if (c == '\n')
  {
    buf[i++] = c;
  }

  buf[i] = '\0';

  return i;
}

char* alloc(int size)
{
  if (allocp + size - allocbuf > MAXALLOCSIZE)
  {
    printf("Out of Memory\n");
    return NULL;
  }

  allocp += size;
  return allocp - size;
}
