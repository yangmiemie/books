// Write a program to determine the ranges of char , short , int , and long variables, both
// signed and unsigned , by printing appropriate values from standard headers and by direct
// computation. Harder if you compute them: determine the ranges of the various floating-point
// types.

#include <stdio.h>
#include <limits.h>

int main(int argc, char const *argv[])
{
  printf("The size of Char: %d\n", CHAR_BIT); 
  printf("The max of Char: %d\n", CHAR_MAX);  
  printf("The min of Char: %d\n", CHAR_MIN);  
  printf("The max of long: %ld\n", LONG_MAX); 
  printf("The min of long: %ld\n", LONG_MIN); 
  printf("The max of int: %d\n", INT_MAX);  
  printf("The min of int: %d\n", INT_MIN);  
  printf("The max of short: %d\n", SHRT_MAX); 
  printf("The min of short: %d\n", SHRT_MIN); 
  printf("The max of unsigned char: %d\n", UCHAR_MAX);  
  printf("The max of unsigned long: %lu\n", ULONG_MAX); 

  return 0;
}