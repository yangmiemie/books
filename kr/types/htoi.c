// Write the function htoi(s) , which converts a string of hexadecimal digits (including an
// optional 0x or 0X) into its equivalent integer value. The allowable digits are 0 through 9, a
// through f, and A through F .

#include <stdio.h>
#define MAXLEN 1024
#define INVALID -1

long my_htoi(char const s[]);
double my_power(int base, int n);

int main(int argc, char const *argv[])
{
  int i;

  for (i = 1; i < argc; ++i)
  {
    printf("%ld\n", my_htoi(argv[i]));
  }

  return 0;
}


long my_htoi(char const s[])
{
  int i, j, len = 0;
  long sum = 0;
  int startStr = 0;

  if (s == NULL)
  {
    return INVALID;
  }

  if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
  {
    startStr = 2;
  }

  for (i = 0; s[i] != '\0'; ++i);
  
  len = i;
  j = 0;

  for (i = len - 1; i >= startStr; --i)
  {
    if (s[i] >= '0' && s[i] <= '9' )
    {
      sum += (s[i] - '0') * my_power(16, j);
    }
    else if (s[i] >= 'a' && s[i] <= 'f')
    {
      sum += (s[i] - 'a' + 10) * my_power(16, j);
    }
    else if (s[i] >= 'A' && s[i] <= 'F')
    {
      sum += (s[i] - 'A' + 10) * my_power(16, j);
    }
    else
    {
      return INVALID;
    }

    ++j;
  }

  return sum;
}

double my_power(int base, int n)
{
  if (base == 0)
  {
    return 0;
  }

  if (n == 0)
  {
    return 1;
  }

  int i = 0, absoluteN;
  double sum = 1;

  absoluteN = n > 0 ? n : (n * -1);

  for (i = 0; i < absoluteN; ++i)
  {
    sum *= base;
  }

  if (n < 0)
  {
    sum = 1.0 / sum;
  }

  return sum;
}
