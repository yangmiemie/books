#include <stdio.h>

#define ISDIGITS(x) ((x >= '0' && x <= '9') ? 1 : 0)
#define ISALPHA(x) (((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')) ? 1 : 0)
#define NUMBER 1
#define IDENTIFIER 2

void ungetch(char c);
int getch();

int getop(char s[])
{
  int i, c;

  while(((c = getch()) == ' ') || (c == '\t'));

  // printf("getop: %c\n", c);

  if (ISALPHA(c))
  {   
    i = 0;
    s[i++] = c;
    for (c = getch(); ISALPHA(c); ++i)
    {
      s[i] = c;
      c = getch();
    }

    if (c != EOF)
    {
      ungetch(c);
    }

    s[i] = '\0';
    return IDENTIFIER;
  }

  if (!ISDIGITS(c))
  {
    return c;
  }


  i = 0;
  s[i++] = c;

  for (c = getch(); ISDIGITS(c); ++i)
  {
    s[i++] = c;
    c = getch();
  }

  if (c == '.')
  {
    s[i++] = '.';
    for (c = getch(); ISDIGITS(c); ++i)
    {
      s[i++] = c;
      c = getch();
    }
  }

  if (c != EOF)
  {
    ungetch(c);
  }

  return NUMBER;
}