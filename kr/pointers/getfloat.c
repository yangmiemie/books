// Write getfloat , the floating-point analog of getint . What type does getfloat return as
// its function value?
#include <stdio.h>

#define MAXLEN 64
#define ISDIGITS(x) ((x >= '0') && (x <= '9') ? 1 : 0)

char buf[MAXLEN];
int bufp;

void ungetch(int c);
int getch(void);
int getfloat(float *pn);

int main(int argc, char const *argv[])
{
  float f;
  getfloat(&f);

  printf("%f\n", f);
  return 0;
}

int getfloat(float *pn)
{
  int c, sign, power = 1;

  while((c = getch()) == ' ');

  while(!ISDIGITS(c) && c != '+' && c != '-')
  {
    ungetch(c);
    return 0;
  }

  sign = (c == '-') ? -1: 1;

  if (c == '-' || c == '+')
  {
    c = getch();
  }

  *pn = 0;
  for(; ISDIGITS(c); c = getch())
  {
    *pn = 10 * *pn + c - '0';
  }

  if (c == '.')
  {
    power = 1;

    for(c = getch(); ISDIGITS(c); c = getch())
    {
      *pn = 10 * *pn + c - '0';
      power *= 10;
    }
  }

  *pn = *pn * sign / power;
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
