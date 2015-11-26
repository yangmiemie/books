// Following the lead of the UNIX program grep, let us enhance the
// program so the pattern to be matched is specified by the first argument on the command line.

#define NULL 0
#define EOF -1
#define MAXLEN 1024

int getline(char s[], int size);
int strlen(const char* s);
char* strstr(const char* s1, const char* s2);

int main(int argc, char const *argv[])
{
  char line[MAXLEN] = {0};
  int len, found;
  int number, except;
  int c;
  int lineno;

  number = 0;
  except = 0;
  while((--argc > 0) && **(++argv) == '-' )
  {
    while((c = *(++(*argv))) != '\0')
    {
      switch(c)
      {
        case 'n':
          number = 1;
          break;
        case 'x':
          except = 1;
          break;
        default:
          printf("Invalid argument\n");
        break;
      }
    }
  }

  found = 0;
  lineno = 0;
  while((len = getline(line, MAXLEN)) > 0)
  {
    ++lineno;
    if ((strstr(line, *argv) != NULL) != except)
    {
      if (number > 0)
      {
        printf("%d: ", lineno);
      }
      printf("%s", line);
      ++found;
    }
  }

  printf("%s has been found in %d times\n", *argv, found);
  return 0;
}

int getline(char s[], int size)
{
  int c, i = 0;

  while(((c = getchar()) != EOF) && (c != '\n') && (i < size))
    s[i++] = c;

  if (c == '\n')
  {
    s[i++] = c;
  }

  s[i] = '\0';

  return i;
}

int strlen(const char* s)
{
  char* ptr;

  if (s == NULL)
  {
    return -1;
  }

  ptr = s;
  while(*ptr != '\0')
    ++ptr;

  return ptr - s;
}

char* strstr(const char* haystack, const char* needle)
{
  char *pStack, *pNeedle, *pStackLoop;

  if (haystack == NULL || needle == NULL)
  {
    return NULL;
  }

  pStack = haystack;
  pNeedle = needle;

  for (; *pStack != '\0'; ++pStack)
  {
    if (*pStack == *pNeedle)
    {
      pStackLoop = pStack;

      while((*pNeedle != '\0') && (*pStackLoop != '\0') && (*pStackLoop == *pNeedle))
      {
        ++pNeedle;
        ++pStackLoop;
      }

      if (*pNeedle == '\0')
      {
        return pStack;
      }
      else
      {
        pNeedle = needle;
      }
    }
  }

  return NULL;
}

