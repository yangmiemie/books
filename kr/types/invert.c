// Write a function invert(x,p,n) that returns x with the n bits that begin at position p
// inverted (i.e., 1 changed into 0 and vice versa), leaving the others unchanged.

#include <stdio.h>

unsigned invert(unsigned x, int p, int n);

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    printf("The input should include two arguments\n");
    return -1;
  }

  int x = atoi(argv[1]);
  
  printf("x = %4x\n", x);
  printf("z = %4x\n", invert(x, 5, 3));
  return 0;
}

unsigned invert(unsigned x, int p, int n)
{
  return x ^ ((~(~0 << n)) << (p + 1 - n));
}