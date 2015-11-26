// Suppose there will never be more than one character of pushback. Modify getch and
// ungetch accordingly.

#include <stdio.h>
int buf = EOF;

void ugetc(char c)
{
  if (buf != EOF)
  {
    printf("There is too much characters in buf\n");
    return;
  }

  buf = c;
}

int getch()
{
  char tmp;
  if (buf != EOF)
  {
    tmp = buf;
    buf = EOF;
    return tmp;
  }

  return getchar();
}

int main(int argc, char const *argv[])
{
  int c;

  while((c = getch()) != EOF)
  {
    if (c == '/')
    {
      putchar(c);
      if ((c = getch()) == '*')
      {
        ugetc('!');
      }
    }
    else
    {
      putchar(c);
    }
  }

  return 0;
}
