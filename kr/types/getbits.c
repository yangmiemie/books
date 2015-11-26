// As an illustration of some of the bit operators, consider the function getbits(x,p,n) that
// returns the (right adjusted) n-bit field of x that begins at position p.

#include <stdio.h>

unsigned getbits(unsigned int x, int p, int n);

int main(int argc, char const *argv[])
{
  unsigned int ui = 426;
  int p = 4, n = 3;

  printf("%x\n", ui);
  printf("%x\n", getbits(ui, p, n));
  return 0;
}

/* getbits: get n bits from position p */
unsigned getbits(unsigned int x, int p, int n)
{
  return (x >> (p + 1 -n)) & ~(~0 << n);
}