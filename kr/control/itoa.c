// Exercise 3-4. In a two's complement number representation, our version of itoa does not
// handle the largest negative number, that is, the value of n equal to -(2wordsize-1). Explain why not.
// Modify it to print that value correctly, regardless of the machine on which it runs.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISDIGITS(x) ((x >= '0' && x <= '9') ? 1 : 0)
#define MAXLEN 1024

int my_atoi(char s[]);
void my_itoa(int n, char s[]);
void my_itoa2(int n, char s[]);
void reverse(char s[]);

int main(int argc, char *argv[])
{
  int type, value;
  char numbers[MAXLEN] = {0};

  if (argc != 3)
  {
    printf("The input should include two arguments\n");
    return -1;  
  }

  type = atoi(argv[1]);

  switch(type)
  {
    case 1:
      printf("atoi(%s) = %d\n", argv[2], my_atoi(argv[2]));
      break;
    case 2:
      value = my_atoi(argv[2]);
      my_itoa2(value, numbers);
      printf("itoa(%d) = %s\n", value, numbers);
      break;
    default:
      break;
  }

  return 0;
}

int my_atoi(char s[])
{
  int i = 0;
  int sign = (s[0] == '-') ? -1 : 1;
  int result = 0;

  // for(i = 0; s[i] != '\0'; ++i);
  // len = i;

  if (s[0] == '+' || s[0] == '-')
  {
    i = 1;
  }

  for (result = 0; ISDIGITS(s[i]); ++i)
  {
    result = result * 10 + s[i] - '0';
  }

  return result * sign;
}

void my_itoa(int n, char s[])
{
  int sign = n > 0 ? 1 : -1;
  unsigned int un;

  if (sign < 0)
  {
    un = n * -1;
  }
  else
  {
    un = n;
  }

  int i = 0;
  while(un > 0)
  {
    s[i] = un % 10 + '0';
    un = un / 10;
    ++i;
  } 

  if (sign < 0)
  {
    s[i++] = '-';
  }

  s[i] = '\0';

  reverse(s);
}

void my_itoa2(int n, char s[])
{
  int sign = n > 0 ? 1 : -1;

  int i = 0;
  while(n != 0)
  {
    s[i++] = abs(n % 10) + '0';
    n = n / 10;
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
