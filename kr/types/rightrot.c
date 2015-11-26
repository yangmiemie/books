// Exercise 2-8. Write a function rightrot(x,n) that returns the value of the integer x rotated
// to the right by n positions.

#include <stdio.h>
int rightrot(int x, int n);
int getBinaryLen(int x);

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    printf("The input should include two arguments\n");
    return -1;
  }

  int x = atoi(argv[1]);
  
  printf("x = %4x\n", x);
  printf("z = %4x\n", rightrot(x, 5));

  return 0;
}

int rightrot(int x, int n)
{
  int len = getBinaryLen(x);

  // printf("%d\n", len);
  // printf("%d\n", (~(~0 << n)));
  // printf("%d\n", ((~(~0 << n)) & x));
  // printf("%x\n", (~(~0 << n) & x) << (len - n));
  // printf("%x\n", x >> n);

  return ((~(~0 << n) & x) << (len - n)) | (x >> n);
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