// Write a program to print a histogram of the frequencies of different characters
// in its input.

#include <stdio.h>
#define CHNUMBER 52

int main(int argc, char const *argv[])
{
  int c;
  int chFrenquency[CHNUMBER] = {0};
  int maxFrequency = 0;
  int currVal = 0;
  int i, j;
  int chSum = 0;

  while ((c = getchar()) != EOF)
  {
    if (c >= 'A' && c <= 'Z')
    {
      currVal = ++chFrenquency[c-'A'];
    }

    if (c >= 'a' && c <= 'z')
    {
      currVal = ++chFrenquency[c-'a'+26];
    }

    if (maxFrequency < currVal)
    {
      maxFrequency = currVal;
    }
  }

  for (i = 0; i < CHNUMBER; ++i)
  {
    chSum += chFrenquency[i];
    printf("%d  ", chFrenquency[i]);
  }

  printf("\nThe total number of characters is %d\n", chSum);
  for (i = maxFrequency; i > 0; --i)
  {
    printf("%4d  |", i);

    for (j = 0; j < CHNUMBER; ++j)
    {
      if (chFrenquency[j] >= i)
      {
        printf("* ");
      }
      else
      {
        printf("  ");
      }
    }
    printf("\n");
  }

  printf("    ---------------------------------------------------------------------------------------------------\n");
  printf("       ");
  for (i = 0; i < CHNUMBER / 2; ++i)
  {
    printf("%c ", i + 'A');
  }
  for (i = CHNUMBER / 2; i < CHNUMBER; ++i)
  {
    printf("%c ", i - 26 + 'a');
  }

  return 0;
}