// Write a program to print a histogram of the lengths of words in its input. It is easy to draw the
// histogram with the bars horizontal; a vertical orientation is more challenging.

#include <stdio.h>
#define OUTWORD 0
#define INWORD  1
#define MAXLENGTH 12

int main(int argc, char const *argv[])
{
  int nLength[MAXLENGTH] = {0};
  int c;
  int state = OUTWORD;
  int length = 0;
  int i, j;
  int done = 0;
  int wordSum = 0;

  // the most frequent value of length
  int maxCurrency = 0;

  while(done == 0)
  {
    c = getchar();
    if (c == ' ' || c == '\n' || c == '\t')
    {
      if (state == INWORD)
      {
        // The words whose length is larger than 10 are stored in nLength[11].
        if (length > 10)
        {
          length = 11;
        }

        ++nLength[length];          
        state = OUTWORD;

        if (maxCurrency < nLength[length])
        { 
          maxCurrency = nLength[length];
        }
      }
    }
    else if (c == EOF)
    {
      // if the eof is the end of a word, then add this word.
      if (state == INWORD)
      {     
        ++nLength[length];
      }

      done = 1;
      
      if (maxCurrency < nLength[length])
      {
        maxCurrency = nLength[length];
      }
    }
    else
    {
      if (state == OUTWORD)
      {
        state = INWORD;
        length = 0;
      }

      ++length;
    }
  }

  for (i = maxCurrency; i > 0; --i)
  {
    printf("%d  | ", i);

    for(j = 0; j < MAXLENGTH; ++j)
    {
      if (nLength[j] >= i)
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

  printf("   ");
  printf("---------------------------------------------\n");
  printf("        ");
  for (i = 1; i < MAXLENGTH; ++i)
  {
    printf("%d ", i);
  }

  printf("\n\n\nmaxCurrency = %d\n", maxCurrency);
  for (i = 1; i < MAXLENGTH; ++i)
  {
    wordSum += nLength[i];
    printf("%d ", nLength[i]);
  }
  printf("\n");
  printf("The totol word number is %d\n", wordSum);
  return 0;
}

