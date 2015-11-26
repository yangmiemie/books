// Write the program expr , which evaluates a reverse Polish expression from the command line,
// where each operator or operand is a separate argument. For example, expr 2 3 4 + * evaluates 2 X (3 + 4).

#define MAXLEN 1024
#define INVALID -1
#define NULL 0
#define ISDIGITS(x) ((x >= '0') && (x <= '9')) ? 1 : 0

double stack[MAXLEN];
int stackp;

int isNumber(char* s);
int strcmp(const char* s1, const char* s2);
void push(double value);
double pop();
int atoi(const char*);
double atof(const char*);

int main(int argc, char *argv[])
{
  char *ptr;
  double op;
  // double op2;

  printf("argc = %d\n", argc);

  while(--argc > 0)
  {
    ptr = *(++argv);

    printf("%s\n", ptr);

    if (isNumber(ptr))
    {
      push(atof(ptr));
    }
    else if (!strcmp(ptr, "+"))
    {
      push(pop() + pop());
    }
    else if (!strcmp(ptr, "-"))
    {
      op = pop();
      push(pop() - op);
    }
    else if (!strcmp(ptr, "*"))
    {
      push(pop() * pop());
    }
    else if (!strcmp(ptr, "/"))
    {
      op = pop();
      push(pop() / op);
    }
    else
    {
      printf("\nInvalid option: %s\n", ptr);
      return;
    }
  }

  printf("The result is %f\n", pop());
  return 0;
}

double atof(const char* nptr)
{
  double n;
  int sign, power;

  sign = (*nptr == '-') ? -1 : 1;

  if (sign == '-' || sign == '+')
  {
    ++nptr;
  }

  n = 0;
  for (; ISDIGITS(*nptr); ++nptr)
  {
    n = n * 10 + *nptr - '0';
  }

  power = 1;  
  if (*nptr == '.')
  {
    ++nptr;

    for (; ISDIGITS(*nptr); ++nptr)
    {
      n = n * 10 + *nptr - '0';
      power *= 10;
    }
  }

  return sign * n / power;
}

int atoi(const char* nptr)
{
  int n = 0;
  int sign;

  sign = (*nptr == '-') ? -1 : 1;

  if (*nptr == '-' || *nptr == '+')
  {
    ++nptr;
  }

  for (; *nptr != '\0'; ++nptr)
  {
    if (!ISDIGITS(*nptr))
    {
      return 0;
    }

    n = 10 * n + *nptr - '0';
  }

  return n * sign;
}

int strcmp(const char* s1, const char* s2)
{
  if (s1 == NULL || s2 == NULL)
  {
    return -1;
  }

  while(*s1 == *s2)
  {
    if (*s1 == '\0')
    {
      return 0;
    }

    ++s1;
    ++s2;
  }

  return *s1 - *s2;
}

int isNumber(char* s)
{
  if (s == NULL)
  {
    return 0;
  }

  while((((*s >= '0') && (*s <= '9')) || (*s == '.')) && (*s != '\0'))
    ++s;

  return *s == '\0';
}

void push(double value)
{
  if (stackp >= MAXLEN)
  {
    printf("The stack is full now\n");
    return;
  }

  printf("%f has been pushed to stack\n", value);
  stack[stackp++] = value;
}

double pop()
{
  if (stackp <= 0)
  {
    printf("The stack is empty now\n");
    return INVALID;
  }

  return stack[--stackp];
}