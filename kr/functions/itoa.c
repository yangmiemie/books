// Adapt the ideas of printd to write a recursive version of atoi ; that is, convert an integer into
// a string by calling a recursive routine.

#include <string.h>

void printd(int n)
{
  if (n < 0)
  {
    putchar('-');
    n = -n;
  }

  if (n / 10)
  {
    printd(n / 10);
  }

  putchar(n % 10 + '0');
}

int atoi(char s[], int size)
{
  if (size > 1)
  {
    return s[size-1] - '0' + 10 * atoi(s, size-1);
  }

  return s[0] - '0';
}

void itoa(int value)
{
  if (value / 10)
  {
    itoa(value / 10);
  }

  putchar(value % 10 + '0');
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("The input should include one argument");
    return -1;
  }

  int value = atoi(argv[1], strlen(argv[1]));

  itoa(value);  
  return 0;
}