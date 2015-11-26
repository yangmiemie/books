// Write a program to remove all trailing blanks and tabs from each line of input, and to delete
// entirely blank lines.

#include <stdio.h>
#define MAXLEN 1024
#define MINLENGTH 20

void trim_blank(char line[], int size);
int get_line(char line[], int size);
void print_line(char line[], int size);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length = 0;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    trim_blank(line, length);
    print_line(line, length);
  }

  return 0;
}

void print_line(char line[], int size)
{
  int i, c;

  for (i = 0; i < size; ++i)
  {
    c = line[i];

    if (c == '\t')
    {
      putchar('\\');
      putchar('t');

      continue;
    }

    if (c == ' ')
    {
      putchar('-');
      putchar('-');

      continue;
    }

    if (c == 8)
    {
      putchar('\\');
      putchar('b');

      continue;
    }

    if (c == '\\')
    {
      putchar('\\');
      putchar('\\');

      continue;
    }

    putchar(c);
  }
}

void trim_blank(char line[], int size)
{
  int i;

  // the only character of the line is '\n', and the line is a blank line.
  if (line[0] == '\n')
  {
    line[0] = '\0';
    return;
  }
  // The last character is '\n', so the operation of trim begin at size - 2
  for (i = size - 2; i >= 0; --i)
  {
    if (line[i] == ' ' || line[i] == '\t')
    {
      line[i] = '\n';
      line[i+1] = '\0';
    }
    else
    {
      break;
    }
  }
  
  if (line[0] == '\n')
  {
    line[0] = '\0';
  }
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
