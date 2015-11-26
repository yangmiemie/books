// Write versions of the library functions strncpy , strncat , and strncmp , which operate on
// at most the first n characters of their argument strings. For example, strncpy(s,t,n) copies
// at most n characters of t to s . Full descriptions are in Appendix B.

#include <stdio.h>

void strcpy(char* s, char* t)
{
  while(*s++ = *t++);
}

int strcmp(char* s, char* t)
{
  while(*s++ == *t++)
  {
    if (*s == '\0')
    {
      return 0;
    }
  }

  return *s - *t;
}

char* strcat(char* dest, char* src)
{
  char* tmp;
  if (dest == 0 && src == 0)
  {
    return 0;
  }

  tmp = dest;

  while(*dest++ != '\0');
  --dest;

  while(*dest++ = *src++);

  return tmp;
}

int strend(char* s, char* t)
{
  char* sStart  = s;
  char* tStart  = t;

  if (s == 0 || t == 0)
  {
    return 0;
  }

  while(*s++);
  while(*t++);

  --s, --t;

  for (; t >= tStart && (s >= sStart) && (*s == *t); --t, --s);

  return t < tStart ? 1 : 0;
}

char* strncpy(char* dest, const char* src, int n)
{
  while((n > 0) && (*src != '\0'))
  {
    *dest++ = *src++;
    n--;
  }

  if (n == 0)
  {
    *dest = '\0';
  }
}

char* strncat(char* dest, const char* src, int n)
{
  char* pDest = dest;
  const char* pSrc = src;

  while(*pDest++ != '\0');
  --pDest;

  while((n-- > 0) && (*pDest++ = *src++));

  if (n == -1)
  {
    *pDest = '\0';
  }

  return dest;
}

int strncmp(const char* dest, const char* src, int n)
{
  while((n-- > 0) && (*dest++ == *src++));

  if (n == 0)
  {
    return 1;
  }
  else
  {
    return *dest - *src;
  }
}

int main(int argc, char *argv[])
{
  char s1[] = "12345678";
  char s2[32];
  char s3[] = "abcdefg";

  strncpy(s2, s1, 4);
  printf("%s\n", s2);

  printf("%d\n", strncmp(s3, s2, 10));
  printf("%s\n", s2);
  printf("%s\n", strncat(s2, s3, 5));
  printf("%s, %s, %d\n", s1, s3, strend(s1, s3));
  return 0;
}