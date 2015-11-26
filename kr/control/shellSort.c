// Shell sort

#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 64

void shellsort(int v[], int size);

int main(int argc, char const *argv[])
{
  int array[MAXLEN] = {0};
  int i;

  for (i = 0; i < MAXLEN; ++i)
  {
    array[i] = rand() / 10000000;
  }

  for (i = 0; i < MAXLEN; ++i)
  {
    printf("%d ", array[i]);
  }
  printf("\n");

  shellsort(array, MAXLEN);

  for (i = 0; i < MAXLEN; ++i)
  {
    printf("%d ", array[i]);
  }
  printf("\n");

  return 0;
}

void shellsort(int v[], int size)
{
  int i, j, k;
  int temp;

  for (k = size / 2; k >= 1; --k)
  {
    for (i = size - 1; i >= 0; i-=k)
    {
      for (j = k; j <= i; j+=k)
      {
        if (v[j-k] > v[j])
        {
          temp = v[j];
          v[j] = v[j-k];
          v[j-k] = temp;
        }
      }
    }
  }
}