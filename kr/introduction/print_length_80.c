// Exercise 1-17. Write a program to print all input lines that are longer than 80 characters.

#include <stdio.h>
#define MAXLEN 1024
#define MINLENGTH 20

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int length = 0;

  while((length = get_line(line, MAXLEN)) > 0)
  {
    if(length >= MINLENGTH)
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
