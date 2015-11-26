// Write a program to remove all comments from a C program. Don't forget to handle quoted
// strings and character constants properly. C comments do not nest.

#include <stdio.h>
#define MAXLEN 1024
#define INQOUTE  1
#define OUTQOUTE 0
#define INSLASH  1
#define OUTSLASH 0
#define INASTERISK 1
#define OUTASTERISK 0

int get_line(char line[], int size);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length;
  int i, lastchar = 0;
  int quoteState = OUTQOUTE;
  int asteriskState = OUTASTERISK;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    for (i = 0; i < length; ++i)
    {
      // after /* , ignore every characters until the */ is found 
      if (asteriskState == INASTERISK)
      {
        if ((line[i] == '*') && (line[i+1] == '/'))
        {
          asteriskState = OUTASTERISK;
          ++i;
        }

        continue;
      }

      if (line[i] == '"')
      {
        quoteState = INQOUTE - quoteState;
      }

      if (line[i] == '/')
      {
        if ((line[i+1] == '/') && (quoteState == OUTQOUTE))
        {
          // if the entire line is comments, do not put a new line.
          if(i != 0)
          {
            putchar('\n');            
          }

          break;
        }

        if (line[i+1] == '*')
        {
          asteriskState = INASTERISK;
          ++i;
          continue;
        }
      }

      putchar(line[i]);
    }

  }

  return 0;
}

int get_line(char line[], int size)
{
  int c, i = 0;
  while(((c = getchar()) != EOF) && (i < size) && (c != '\n'))
  {
    line[i] = c;
    ++i;
  }

  if (c == '\n')
  {
    line[i] = '\n';
    ++i;
  }

  line[i] = '\0';

  return i;
}

