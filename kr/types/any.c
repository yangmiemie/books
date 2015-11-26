// Write the function any(s1,s2) , which returns the first location in the string s1 where any
// character from the string s2 occurs, or -1 if s1 contains no characters from s2 . (The standard
// library function strpbrk does the same job but returns a pointer to the location.)

#include <stdio.h>
int getIndexOfString(char target, char const str[]);
int any(char const s1[], char const s2[]);
int any_one_loop(char const s1[], char const s2[]);

int main(int argc, char const *argv[])
{
  if (argc != 3)
  {
    printf("The input should include two arguments\n");
    return -1;
  } 

  printf("%d\n", any_one_loop(argv[1], argv[2]));
  return 0;
}

int any_one_loop(char const s1[], char const s2[])
{
  if (s1 == NULL || s2 == NULL)
  {
    return -1;
  }
  
  char array[256] = {0};
  int i;

  for (i = 0; s2[i] != '\0'; ++i)
  {
    array[s2[i]] = 1;
  }

  for (i = 0; s1[i] != '\0'; ++i)
  {
    if (array[s1[i]] == 1)
    {
      return i;
    }
  }

  return -1;
}

int any(char const s1[], char const s2[])
{
  int i, pos;

  if (s1 == NULL || s2 == NULL)
  {
    return -1;
  }

  for (i = 0; s1[i] != '\0'; ++i)
  {
    if ((pos = getIndexOfString(s1[i], s2)) != -1)
    {
      return pos;
    }
  }

  return -1;
}

int getIndexOfString(char target, char const str[])
{
  int i;

  for (i = 0; str[i] != '\0'; ++i)
  {
    if (target == str[i])
    {
      return i;
    }
  }

  return -1;
}