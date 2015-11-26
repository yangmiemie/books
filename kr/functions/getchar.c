#include <stdio.h>

#define MAXLEN 64

char chbuf[MAXLEN];
int bufp = 0;

void ungetch(char c)
{
  if (bufp > MAXLEN)
  {
    printf("The character buffer is full\n");
    return;
  }

  chbuf[bufp++] = c;
}

int getch()
{
  return bufp > 0 ? chbuf[--bufp] : getchar();  
}