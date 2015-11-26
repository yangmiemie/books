// Exercise 3-5. Write the function itob(n,s,b) that converts the integer n into a base b
// character representation in the string s. In particular, itob(n,s,16) formats s as a
// hexadecimal integer in s.

#include <stdio.h>
#include <string.h>

#define MAXLEN 64
int itob(int n, char s[], int b);
void reverse(char s[]);

int main(int argc, char const *argv[])
{
  char result[MAXLEN] = {0};

  if (argc != 3)
  {
    printf("The input should include three arguments\n");
    return -1;  
  }

  int value = atoi(argv[1]);
  int base = atoi(argv[2]);

  itob(value, result, base);

  printf("value = %d, base = %d, converted value = %s\n", value, base, result);
  return 0;
}

int itob(int n, char s[], int b)
{
  char digits[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i = 0;
  int sign = n > 0 ? 1 : -1;
  char c;

  while(n != 0)
  {
    s[i++] = digits[abs(n % b)];
    n = n / b;
  }

  if (sign < 0)
  {
    s[i++] = '-';
  }

  s[i] = '\0';
  reverse(s);
}

void reverse(char s[])
{
  int i = 0, j = strlen(s) - 1;
  int tmp = 0;

  while(i < j)
  {
    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;

    ++i;
    --j;
  }
}
