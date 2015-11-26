// Write a recursive version of the function reverse(s) , which reverses the string s in place.
#include <string.h>

void swap(char* s1, char* s2)
{
  char tmp;
  tmp = *s1;
  *s1 = *s2;
  *s2 = tmp;
}

void reverse(char s[], int size)
{
  if (size == 1 || size == 0)
  {
    return;
  }
  else
  {
    swap(s, s + size - 1);
    reverse(s + 1, size - 2);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("The input should include one argument");
    return -1;
  }

  // char s[64] = {0};
  // int i;

  // for (i = 0; argv[1][i] != '\0'; ++i)
  // {
  //  s[i] = argv[1][i];
  // }
  // s[i] = '\0';

  // printf("%s\n", s);
  reverse(argv[1], strlen(argv[1]));

  printf("%s\n", argv[1]);
  return 0;
}