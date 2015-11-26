// The sorting process has three steps:
// read all the lines of input
// sort them
// print them in order

// #include <stdio.h>
#define MAXLINES 1024
#define ALLOCSIZE 1024 * 32
#define EOF -1

char allocbuf[ALLOCSIZE];
char* allocp = allocbuf;
char *lineptr[MAXLINES];

int readlines(char* lineptr[], int nlines);
void writelines(char* lineptr[], int nlines);
int getline(char*, int);
char *alloc(int size);
void strcpy(char* dest, char* src);
void qsort(char* v[], int left, int right);
void swap(char* v[], int i, int j);
int strcmp(char* s, char* t);

int main(int argc, char const *argv[])
{
  int lines;

  // char* s1 = "abcdefgh";
  // char* s2 = "1234567";

  // char* array[] = {
  //  s1, s2
  // };

  // qsort(array, 0, 1);
  // writelines(array, 2);
  while((lines = readlines(lineptr, MAXLINES)) > 0)
  {
    qsort(lineptr, 0, lines - 1);
    writelines(lineptr, lines);
  }

  return 0;
}

int strcmp(char* s, char* t)
{
  for(; *s == *t; ++s, ++t)
  {
    if (*s == '\0')
    {
      return 0;
    }
  }

  return *s - *t;
}

void swap(char* v[], int i, int j)
{
  char* tmp = v[i];
  v[i] = v[j];
  v[j] = tmp;
}

void qsort(char* v[], int left, int right)
{
  int last, i;

  if (left >= right)
  {
    return;
  }

  swap(v, left, (left + right) / 2);
  last = left;

  for (i = left + 1; i <= right; ++i)
  {
    if (strcmp(v[left], v[i]) > 0)
    {
      swap(v, ++last, i);
    }
  }

  swap(v, left, last);
  qsort(v, left, last-1);
  qsort(v, last+1, right);
}

void writelines(char* lineptr[], int nlines)
{
  int i;
  for (i = 0; i < nlines; ++i)
  {
    printf("%s", lineptr[i]);
  }
}

int readlines(char* lineptr[], int nlines)
{
  int i = 0, size;
  char *ptr;
  char buf[ALLOCSIZE] = {0};

  while((size = getline(buf, ALLOCSIZE)) > 0)
  {
    // printf("readlines size %d,get %s", size, buf);

    if (i > nlines)
    {
      printf("The lineptr is full now\n");
      return nlines;
    }

    ptr = alloc(size + 1);
    // ptr[size-1] = '\0';
    // printf("ptr is %d\n", ptr);
    strcpy(ptr, buf);
    lineptr[i++] = ptr;
  }

  return i;
}

void strcpy(char* dest, char* src)
{
  while(*dest++ = *src++);
}


char *alloc(int size)
{
  char *ptr;

  if (allocp + size - allocbuf > ALLOCSIZE)
  {
    printf("The allocbuf is full now\n");
    return 0;
  }
  
  ptr = allocp;
  allocp += size;
  return ptr;
}

int getline(char* buf, int size)
{
  int c, i = 0;

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