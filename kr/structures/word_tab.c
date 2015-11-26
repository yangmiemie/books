#include <stdio.h>

#define MAXWORD 64
#define ISALPHA(x) ((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z'))
#define ISALNUM(x) ((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')) || ((x >= '0') && (x <= '9'))
#define NKEYS 9

struct key {
  char* word;
  int count;
} keytab[] = {
  "break", 0,
  "case", 0,
  "char", 0,
  "const", 0,
  "continue", 0,
  "define", 0,
  "int", 0,
  "void", 0,
  "return", 0
};

char buf[MAXWORD];
char* bufp = buf;
int getch();
void ugetch(char c);

int binsearch(char* word, struct key tabs[], int n);
int getword(char*, int);

int main(int argc, char const *argv[])
{
  int len, n, i;
  char word[MAXWORD];


  printf("%d\n", sizeof("abcd"));

  return 0;
  
  while((len = getword(word, MAXWORD)) >= 0)
  {
    if (ISALPHA(word[0]))
    {
      if ((n = binsearch(word, keytab, NKEYS)) > 0)
      {
        keytab[n].count++;
      }
    }
  }

  for (i = 0; i < NKEYS; ++i)
  {
    printf("%s: %d\n", keytab[i].word, keytab[i].count);
  }

  return 0;
}

int strcmp(char *s1, char *s2)
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

int binsearch(char* word, struct key tabs[], int n)
{
  int left, right, mid, cmp;

  left = 0;
  right = n - 1;

  while(left <= right)
  {
    mid = (left + right) / 2;

    if ((cmp = strcmp(word, tabs[mid].word)) < 0)
    {
      right -= 1;
    }
    else if (cmp > 0)
    {
      left += 1;
    }
    else
    {
      return mid;
    }
  }

  return -1;
}

int getword(char* buf, int size)
{
  int c, i;

  while((c = getch()) == ' ')
    ;

  if (c == EOF)
  {
    return -1;
  }

  if (!ISALPHA(c))
  {
    *buf = '\0';
    return 0;
  }

  *buf++ = c;
  i = 1;
  while((c = getch()) && (i < size))
  {
    if (ISALNUM(c))
    {
      *buf++ = c;
    }
    else
    {
      ugetch(c);
      break;
    }

    ++i;
  }

  *buf = '\0';
  return i;
}

void ugetch(char c)
{
  if (bufp - buf >= MAXWORD)
  {
    printf("BUffer is full\n");
    return;
  }

  *bufp++ = c;
}

int getch()
{
  return (bufp == buf) ? getchar() : *--bufp;
}