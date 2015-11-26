// Exercise 2-9. In a two's complement number system, x &= (x-1) deletes the rightmost 1-bit
// in x. Explain why. Use this observation to write a faster version of bitcount.

#include <stdio.h>

int bitcount(int x);

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    printf("The input should include two arguments\n");
    return -1;
  }

  int x = atoi(argv[1]);
  
  printf("x = %4x\n", x);
  printf("z = %4x\n", bitcount(x));
  
  return 0;
}

int bitcount(int x)
{
  int i;
  for (i = 0; x > 0; x &= (x - 1))
  {
    ++i;
  }

  return i;
}