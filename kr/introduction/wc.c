// wc

#include <stdio.h>

#define INWORD  1
#define OUTWORD 0

int main(int argc, char const *argv[])
{
  int c;
  int state = 0;
  int lineNumber = 0, wordNumber = 0, chNumber = 0;
  int lastchar = 0;
  int done = 0;

  while (done == 0)
  {
    c = getchar();

    if ((c == '\t') || (c == ' ') || (c == '\n'))
    {
      state = OUTWORD;
    }
    else
    {
      // The character before is a space, so this is the begining of a word.
      if (state == OUTWORD)
      {
        wordNumber++;
        state = INWORD;
      }
    }

    if (c == '\n')
    {
      lineNumber++;
    }

    if (c == EOF)
    {
      // The last line eneed with a EOF and this line should be counted.
      if (lastchar != '\n')
      {
        lineNumber++;
      }

      done = 1;

      continue;
    }

    lastchar = c;
    chNumber++;
  }

  printf("lineNumber: %d, wordNumber: %d, chNumber: %d\n", lineNumber, wordNumber, chNumber);
  return 0;
}