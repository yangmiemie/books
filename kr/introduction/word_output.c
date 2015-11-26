// Write a program that prints its input one word per line.

#include <stdio.h>

#define OUTWORD 0
#define INWORD  1

int main(int argc, char const *argv[])
{
  int c, state;

  while ((c = getchar()) != EOF)
  {
    if ((c == ' ') || (c == '\t') || (c == '\n'))
    {
      if (state != OUTWORD)
      {
        putchar('\n');
        state = OUTWORD;
      }

      continue;
    }
    else
    {
      state = INWORD;
    }

    putchar(c);
  }

  return 0;
}