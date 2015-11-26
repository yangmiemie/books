// Exercise 1-22. Write a program to ``fold'' long input lines into two or more shorter lines after
// the last non-blank character that occurs before the n-th column of input. Make sure your
// program does something intelligent with very long lines, and if there are no blanks or tabs
// before the specified column.

#include <stdio.h>
#define MAXLEN 1024
#define FOLDLEN 40

int get_line(char line[], int size);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length, i, spaceIndex, lineIndex;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    if (length > FOLDLEN)
    {
      spaceIndex = 0;
      lineIndex = 0;
      for (i = 0; i < length; ++i)
      {
        if (line[i] == ' ' || line[i] == '\t')
        {
          spaceIndex = i;
        }

        if (lineIndex > FOLDLEN)
        {
          line[spaceIndex] = '\n';
          lineIndex = i - spaceIndex;
        }

        ++lineIndex;
      }
    }

    printf("%s", line);
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