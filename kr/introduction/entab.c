// Write a program entab that replaces strings of blanks with the minimum number of tabs and
// blanks to achieve the same spacing. Use the same stops as for detab . When either a tab or a
// single blank would suffice to reach a tab stop, which should be given preference?

#include <stdio.h>
#define MAXLEN 1024
#define TAB2SPACE 8

int get_line(char line[], int size);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length, spaceNum;
  int i, j;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    spaceNum = 0;
    for (i = 0; i < length; ++i)
    {
      if (line[i] == ' ')
      {
        ++spaceNum;
      }
      else
      {
        for (j = 0; j < spaceNum; ++j)
        {
          putchar(' ');
        }
        putchar(line[i]);
        spaceNum = 0;
      }

      // the index of the end of tab is 7, 15, 23, 31.... 
      if (((i + 1) % TAB2SPACE == 0) && (spaceNum >= 2))
      {
        putchar('\t');
        spaceNum = 0;
      }
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