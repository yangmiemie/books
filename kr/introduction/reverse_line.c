// Exercise 1-19. Write a function reverse(s) that reverses the character string s. Use it to
// write a program that reverses its input a line at a time.
#include <stdio.h>
#define MAXLEN 1024

void reverse_line(char line[], int size);
void swap(char* a, char* b);
int get_line(char line[], int size);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    reverse_line(line, length);
    printf("%s", line);
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

// The line includes a '\n' in the end.
void reverse_line(char line[], int size)
{
  int i, j;

  if (size < 2)
  {
    return;
  }

  i = 0;
  j = size -2;

  while(i < j)
  {
    swap(&line[i], &line[j]);
    ++i;
    --j;
  }
}

void swap(char* a, char* b)
{
  int tmp = *b;
  *b = *a;
  *a = tmp;
}