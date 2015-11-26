// Write a routine ungets(s) that will push back an entire string onto the input. Should ungets
// know about buf and bufp , or should it just use ungetch ?

#define MAXLEN 1024
#define EOF -1
char buf[MAXLEN];
int bufp = 0;

int getc()
{
  return bufp > 0 ? buf[--bufp] : EOF;
}

void ungetc(char c)
{
  if (bufp > MAXLEN)
  {
    printf("There is too much characters in buf\n");
    return;
  }

  buf[bufp++] = c;
}

void ungets(char s[])
{
  int i;
  for (i = 0; s[i] != '\0'; ++i)
  {
    ungetc(s[i]);
  }
}

int main(int argc, char const *argv[])
{
  char s[] = "Write a routine ungets(s) that will push back an entire string onto the input.";
  char c;

  ungets(s);

  while((c = getc()) != EOF)
  {
    putchar(c);
  }

  return 0;
}