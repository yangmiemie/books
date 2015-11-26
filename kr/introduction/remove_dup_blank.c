// Exercise 1-9. Write a program to copy its input to its output, replacing each string of one or
// more blanks by a single blank.

#include <stdio.h>

int main(int argc, char const *argv[])
{
  int lastchar, c;

  while ((c = getchar()) != EOF)
  {
    if (c == ' ' && lastchar == ' ')
    {
      continue;
    }

    putchar(c);
    lastchar = c;
  }
  return 0;
}