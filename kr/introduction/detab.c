// Write a program detab that replaces tabs in the input with the proper number
// of blanks to space to the next tab stop. Assume a fixed set of tab stops, say every n columns.
// Should n be a variable or a symbolic parameter?
#include <stdio.h>
#define MAXLEN 1024

int get_line(char line[], int size);
int getSpaceNumber(int i, int tabSize);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length;
  int tabSize = 8;
  int i, j, spaceNum, newLineIndex = 0;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    newLineIndex = 0;
    for (i = 0; i < length; ++i)
    {
      if (line[i] == '\t')
      {
        // The index of newline should be stored and cannot use i directly
        // spaceNum = getSpaceNumber(i, tabSize); is wrong
        
        spaceNum = getSpaceNumber(newLineIndex, tabSize);
        for (j = 0; j < spaceNum; ++j)
        {
          ++newLineIndex;
          putchar(' ');
        }
      }
      else
      {
        ++newLineIndex;
        putchar(line[i]);
      }
    }
  } 
  return 0;
}

// The column index of the tab end is 8 * N.
int getSpaceNumber(int i, int tabSize)
{
  return tabSize - i % tabSize;
}

int get_line(char line[], int size)
{
  int c = 0, i = 0;

  while(((c = getchar()) != '\n') && (i < size) && (c != EOF))
  {
    line[i] = c;
    ++i;
  }

  if (c == '\n')
  {
    line[i] = c;
    ++i;
  }
  
  line[i] = '\0';

  return i;
}