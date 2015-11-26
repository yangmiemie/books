// Revise the main routine of the longest-line program so it will correctly print the
// length of arbitrary long input lines, and as much as possible of the text.

#include <stdio.h>

#define MAXLEN 1024
int get_line(char line[], int size);

int main(int argc, char const *argv[])
{
  int inputLen;
  char line[MAXLEN] = {0};
  int length = 0;

  if (argc != 2)
  {
    printf("Input error: one argument should be input\n");
    return -1;
  }

  inputLen = atoi(argv[1]);

  while((length = get_line(line, MAXLEN)) > 0)
  {
    if (length == inputLen)
    {
      printf("%s\n", line);
    }
  }

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
