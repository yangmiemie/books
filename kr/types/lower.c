// Rewrite the function lower, which converts upper case letters to lower case, with a conditional
// expression instead of if-else .

#include <stdio.h>

#define MAXLEN 1024

void lower(char s[]);
void my_copy(char const from[], char to[]);

int main(int argc, char const *argv[])
{

  if (argc != 2)
  {
    printf("The input should include two arguments\n");
    return -1;
  }

  char s[MAXLEN] = {0};
  my_copy(argv[1], s);
  
  printf("s = %s\n", s);
  lower(s);
  printf("z = %s\n", s);
  return 0;
}

void my_copy(char const from[], char to[])
{
  int i;
  for (i = 0; from[i] != '\0'; ++i)
  {
    to[i] = from[i];
  }

  to[i] = '\0';
}

void lower(char s[])
{
  int i;
  for (i = 0; s[i] != '\0'; ++i)
  {
    s[i] = (s[i] >= 'A' && s[i] <= 'Z') ? (s[i] + 'a' - 'A') : s[i];
  }
}