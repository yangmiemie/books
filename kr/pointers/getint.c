#include <stdio.h>

#define MAXLEN 64
#define ISDIGITS(x) ((x >= '0') && (x <= '9') ? 1 : 0)

char buf[MAXLEN];
int bufp;

void ungetch(int c);
int getch(void);
int getint(int *pn);

int main(int argc, char *argv[])
{
  int i;

  getint(&i);

  printf("%d\n", i);
  return 0;
}

int getint(int *pn)
{
  int sign;
  int c;

  while((c = getch()) == ' ');

  if (!ISDIGITS(c) && c != '+' && c != '-')
  {
    ungetch(c);
    return 0;
  }

  sign = (c == '-') ? -1 : 1;

  if (c == '-' || c == '+')
  {
    c = getch();
  }

  *pn = 0;

  for(; ISDIGITS(c); c = getch())
  {
    *pn = 10 * *pn + c - '0';
  }

  *pn = *pn * sign;
  return 1;
}

void ungetch(int c)
{
  if (bufp >= MAXLEN)
  {
    printf("The buf is full now\n");
    return;
  }

  buf[bufp++] = c;
}

int getch(void)
{
  return bufp > 0 ? buf[--bufp] : getchar();
}
