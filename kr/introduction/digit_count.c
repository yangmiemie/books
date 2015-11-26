// write a program to count the number of occurrences of each digit, of white space
// characters (blank, tab, newline), and of all other characters.

#include <stdio.h>
#define NUMBER 10

int main(int argc, char const *argv[])
{
  int ndigits[NUMBER] = {0};
  int nSpace = 0, nCh = 0;
  int c;

  while ((c = getchar()) != EOF)
  {
    if (c > '0' && c < '9')
    {
      ++ndigits[c-'0'];
    }
    else if (c == ' ' || c == '\t' || c == '\n')
    {
      ++nSpace;
    }
    else
    {
      ++nCh;
    }
  }

  int i;
  for (i = 0; i < NUMBER; ++i)
  {
    printf("%d ", ndigits[i]);
  }
  printf("\n");

  printf("%d\n", nSpace);
  printf("%d\n", nCh);

  return 0;
}