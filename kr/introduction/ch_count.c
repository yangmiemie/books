#include <stdio.h>

int main(int argc, char const *argv[])
{
  int c;
  int blankNumber = 0, tabNumber = 0, lineNumber = 0;
  int done = 0;
  int lastchar = 0;

  while (done == 0)
  {
    c = getchar();

    if ( c == '\n')
    {
      lineNumber++;
    }
    if ( c == ' ')
    {
      blankNumber++;
    }
    if ( c == '\t' )
    {
      tabNumber++;
    }

    // To make sure the current line that EOF occurred is counted
    if (c == EOF )
    {
      if (lastchar != '\n')
      {
        lineNumber++;
      }
      done = 1;
    }

    lastchar = c;
  }

  printf("Blank: %d, Tab: %d, Line: %d\n", blankNumber, tabNumber, lineNumber);
  return 0;
}