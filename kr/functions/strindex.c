// For now, the pattern to be searched
// for is a literal string, which is not the most general of mechanisms. 

#include <stdio.h>

#define MAXLEN 512

int get_line(char s[], int size);
int strindex(char source[], char target[]);
int strRightMostIndex(char source[], char target[]);

int main(int argc, char *argv[])
{
  char array[MAXLEN] = {0};
  int found = 0;
  int pos = -1;

  while(get_line(array, MAXLEN) > 0)
  {
    pos = strRightMostIndex(array, argv[1]);
    if (pos >= 0)
    {
      printf("%s: %d\n", array, pos);
      ++found;
    }

  }

  printf("The pattern '%s' has been found in %d lines\n", argv[1], found);
  return 0;
}

int get_line(char s[], int size)
{
  int c, i;

  i = 0;
  while(((c = getchar()) != EOF) && (c != '\n') && (i < size))
  {
    s[i++] = c;
  }

  if (c == '\n')
  {
    s[i++] = c;
  }

  s[i] = '\0';

  return i;
}

int strRightMostIndex(char source[], char target[])
{
  int i, j, k;
  int pos = -1;

  for (j = 0; source[j] != '\0'; ++j)
  {
    for (i = 0, k = j; (target[i] != '\0') && (source[k] == target[i]); ++i, ++k);

    if (target[i] == '\0' && i > 0)
    {
      pos = j;
    }
  }

  return pos;
}

int strindex(char source[], char target[])
{
  int i, j, k;
  int found = 0;

  for (j = 0; source[j] != '\0'; ++j)
  {
    for (i = 0, k = j; (target[i] != '\0') && (source[k] == target[i]); ++i, ++k);

    if (target[i] == '\0' && i > 0)
    {
      return j;
    }
  }

  return -1;
}

// int strindex(char source[], char target[])
// {
//  int i, j, k;
//  int found = 0;

//  for (j = 0; source[j] != '\0'; ++j)
//  {
//    if (source[j] == target[0])
//    {
//      k = j + 1;
//      for (i = 1; (target[i] != '\0') && (source[k] != '\0') && (source[k] == target[i]); ++i, ++k);

//      if (target[i] == '\0')
//      {
//        ++found;
//      }
//    }
//  }

//  return found;
// }