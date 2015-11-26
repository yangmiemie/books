// Write a function expand(s1,s2) that expands shorthand notations like a-z in the string s1
// into the equivalent complete list abc...xyz in s2 . Allow for letters of either case and digits,
// and be prepared to handle cases like a-b-c and a-z0-9 and -a-z . Arrange that a leading or
// trailing - is taken literally.

#include <stdio.h>

#define MAXLEN 128
#define ISDIGITS(x) (((x >= '0') && (x <= '9')) ? 1 : 0)
#define ISLOWCH(x) (((x >= 'a') && (x <= 'z')) ? 1 : 0)
#define ISHIGHCH(x) (((x >= 'A') && (x <= 'Z')) ? 1 : 0)

void expand(char s1[], char s2[]);
int expandString(char s[], char from, char to, int start);
void clearString(char s[], int size);

int main(int argc, char *argv[])
{
  char s2[MAXLEN] = {0};
  char *s1[] = { "a-z-", "z-a-", "-1-6-", "a-ee-a", "a-R-L", "1-9-1", "5-5", NULL };
  int i = 0;

  expand(argv[1], s2);
  printf("Unexpanded: %s\n", argv[1]);
  printf("Expanded : %s\n", s2);

  while ( s1[i] ) {
    clearString(s2, MAXLEN);
    expand(s1[i], s2);
    printf("Unexpanded: %s\n", s1[i]);
    printf("Expanded : %s\n", s2);
    ++i;
  }

  return 0;
}

void clearString(char s[], int size)
{
  int i;
  for (i = 0; i < size; ++i)
  {
    s[i] = 0;
  }
}

void expand(char s1[], char s2[])
{
  if (s1 == NULL)
  {
    return;
  }

  int i = 0;
  int j = 0;
  int k;

  while(s1[i] != '\0')
  {
    if (s1[i] == '-')
    {
      if (i == 0 || s1[i+1] == '\0')
      {
        s2[j++] = s1[i++];
      }
      else
      {
        if ((ISDIGITS(s1[i-1]) && ISDIGITS(s1[i+1])) || (ISLOWCH(s1[i-1]) && ISLOWCH(s1[i+1])) || (ISHIGHCH(s1[i-1]) && ISHIGHCH(s1[i+1])))
        {
          j = expandString(s2, s1[i-1], s1[i+1], j - 1);
          i += 2;
        }
        else
        {
          s2[j++] = s1[i++];
        }
      }
    }
    else
    {
      s2[j++] = s1[i++];
    }
  } 
}

// return the last character after expand
int expandString(char s[], char from, char to, int start)
{
  int i = start;

  if (from <= to)
  {
    while(from <= to)
    {
      s[i++] = from;
      ++from;
    }
  }
  else
  {
    while(from >= to)
    {
      s[i++] = from;
      --from;
    }
  }

  return i;
}