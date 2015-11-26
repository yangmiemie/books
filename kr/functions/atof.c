// To illustrate how to deal with this, let us write and use the function atof(s), which converts the string s to its double-
// precision floating-point equivalent. 

#include <stdio.h>
#define ISDIGITS(x) ((x >= '0' && x <= '9') ? 1 :  0)
double atof(char s[]);
double exponentValue(char s[]);

int main(int argc, char *argv[])
{
  double result;

  if (argc != 2)
  {
    printf("The input should include one argument\n");
    return -1;
  }

  result =  atof(argv[1]);
  printf("The input %s is converted to %f\n", argv[1], result);

  return 0;
}

double atof(char s[])
{
  double value, exponent = 1;
  int i, sign, power;

  if (s == NULL)
  {
    return 0;
  }

  sign = s[0] == '-' ? -1 : 1;

  if (s[0] == '+' || s[0] == '-')
  {
    ++i;
  } 

  value = 0;
  for (i = 0; ISDIGITS(s[i]); ++i)
  {
    value = value * 10 + s[i] - '0';
  }

  if (s[i] == '.')
  {
    ++i;
  }

  power = 1;
  for (; ISDIGITS(s[i]); ++i)
  {
    value = value * 10 + s[i] - '0';
    power *= 10;
  }

  if (s[i] == 'e' || s[i] == 'E')
  {
    ++i;
    exponent = exponentValue(s + i);
  }


  return value / power * exponent;
}

double exponentValue(char s[])
{
  int exponent = 0, i = 0;

  double sign, value;

  if (s[0] == '-')
  {
    sign = -1;
  }
  else if (s[0] == '+')
  {
    sign = 1;
  }
  else
  {
    return 0;
  }

  for (i = 1; s[i] != '\0'; ++i)
  {
    exponent = exponent * 10 + s[i] - '0';
  }

  for (value = 1, i = 0; i < exponent; ++i)
  {
    value *= 10;
  }

  return sign > 0 ? value : 1 / value;
}

