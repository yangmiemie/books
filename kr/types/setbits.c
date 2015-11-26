// Write a function setbits(x,p,n,y) that returns x with the n bits that begin at position p
// set to the rightmost n bits of y, leaving the other bits unchanged.

#include <stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y);

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("The input should include two arguments\n");
    return -1;
  }

  int x = atoi(argv[1]);
  int y = atoi(argv[2]);
  
  printf("x = %4x\n", x);
  printf("y = %4x\n", y);
  printf("z = %4x\n", setbits(x, 5, 3, y));
  return 0;
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
  int len = getBinaryLen(x);

  printf("%d\n", len);
  printf("%x\n", ((~(~0 << n)) & y));

  return x | (((~(~0 << n)) & y) << (p + 1 - n));
}

int getBinaryLen(int x)
{
  int i = 0;
  while(x)
  {
    x = x >> 1;
    ++i;
  }

  return i;
}