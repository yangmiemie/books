// reads a set of text lines and prints the longest.

#include <stdio.h>

#define MAXLEN 64
int get_line(char line[], int size);
void copy_line(char from[], char to[]);

int main(int argc, char const *argv[])
{
  char line[1024] = {0};
  char longestLine[1024] = {0};
  int length = 0, maxLength = 0;
  
  while((length = get_line(line, MAXLEN)) > 0)
  {
    if (length > maxLength)
    {
      copy_line(line, longestLine);
      maxLength = length;
    }
  }

  printf("The length of the longestLine is %d\n", maxLength);
  printf("%s\n", longestLine);
  return 0;
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


void copy_line(char from[], char to[])
{
  int i;
  for (i = 0; from[i] != '\0'; ++i)
  {
    to[i] = from[i];
  }

  to[i] = '\0';
}