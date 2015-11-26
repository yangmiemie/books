// Write the program tail, which prints the last n lines of its input. By default, n is 10, say, but
// it can be changed by an optional argument, so that
// tail -n
// prints the last n lines. The program should behave rationally no matter how unreasonable the
// input or the value of n. Write the program so it makes the best use of available storage; lines
// should be stored as in the sorting program of Section 5.6, not in a two-dimensional array of
// fixed size.

#include <stdio.h>

#define MAXLEN 1024 * 64
#define MAXLINELEN 1024
// #define NULL 0
// #define EOF -1

static char allocbuf[MAXLEN];
static char *allocp = allocbuf;

int get_line(char buf[], int size);
char* alloc(int n);
int atoi(char* str);
void strcpy(char* dest, char* src);

int main(int argc, char *argv[])
{
  int numlines, linelen, currentno;
  int i;
  char *lineptr, linebuf[MAXLINELEN];

  if (argc > 1)
  {
    numlines = atoi(argv[1]);

    if (numlines > 0)
    {
      printf("Error Line value\n");
      return 0;
    }

    numlines = -numlines;
  }
  else
  {
    numlines = 10;
  }

  // printf("numlines = %d\n", numlines);

  lineptr = alloc(numlines * MAXLINELEN);

  // printf("alloc finished: %d\n", lineptr);

  currentno = 0;
  while((linelen = get_line(linebuf, MAXLINELEN)) > 0)
  {
    // printf("lineptr index: %d\n", MAXLINELEN * (currentno % numlines));
    strcpy(lineptr + MAXLINELEN * (currentno % numlines), linebuf);
    // printf("lineptr: %s", lineptr + MAXLINELEN * (currentno % numlines));
    ++currentno;
    // printf("currentno = %d\n", currentno);
  }

  // printf("In the end, currentno = %d\n", currentno);

  numlines = numlines < currentno ? numlines : currentno;

  for (i = 0; i < numlines; ++i)
  {
    printf("%s", lineptr + ((currentno + i) % numlines )* MAXLINELEN);
  }

  return 0;
}

void strcpy(char* dest, char* src)
{
  while(*dest++ = *src++)
    ;
}

int atoi(char* str)
{
  int sign, value;

  sign = (*str == '-') ? -1 : 1;

  if (*str == '+' || *str == '-')
  {
    ++str;
  }

  value = 0;
  while(*str != '\0')
  {
    value = value * 10 + *str - '0';
    ++str;
  }

  return value * sign;
}

int get_line(char buf[], int size)
{
  int c, i;

  // printf("In get_line start\n");

  i = 0;
  while(((c = getchar()) != EOF) && (c != '\n') && (i < size))
  {
    buf[i++] = c;
  }

  // printf("In get_line while end\n");

  if (c == '\n')
  {
    buf[i++] = '\n';
  }

  buf[i] = '\0';

  // printf("get_line len = %d\n", i);
  // printf("%s", buf);
  return i;
}

char* alloc(int n)
{
  if (allocp + n - allocbuf > MAXLEN)
  {
    printf("The Memory is exhausted\n");
    return NULL;
  }

  allocp += n;
  return allocp - n;  
}