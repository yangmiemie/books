// Exercise 3-6. Write a version of itoa that accepts three arguments instead of two. The third
// argument is a minimum field width; the converted number must be padded with blanks on the
// left if necessary to make it wide enough.


#include <stdio.h>
#include <string.h>
#define MAXLEN 64

void itob(int n, char s[], int b);
void reverse(char s[]);

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("The input should include three arguments\n");
    return -1;  
  }

  int value = atoi(argv[1]);
  int width = atoi(argv[2]);
  char result[MAXLEN] = {0};

  itob(value, result, width);
  printf("value = %d, width = %d, result = %s\n", value, width, result);

  return 0;
}

void itob(int n, char s[], int b)
{
  int sign = n > 0 ? 1 : -1;
  int i = 0;

  while(n != 0)
  {
    s[i++] = abs(n % 10) + '0';
    n /= 10;
  }

  if (sign < 0)
  {
    s[i++] = '-';
  }

  while (i < b)
  {
    s[i++] = ' ';
  }

  s[i] = '\0';

  reverse(s);
}

void reverse(char s[])
{
  int i = 0, j = strlen(s) - 1;
  int tmp = 0;

  while(i < j)
  {
    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;

    ++i;
    --j;
  }
}
