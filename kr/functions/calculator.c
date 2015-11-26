// The problem is to write a calculator program that provides the operators +, -, * and /. Because it is easier to implement, the calculator will
// use reverse Polish notation instead of infix. 

#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAXLEN 64
#define NUMBER 1
#define IDENTIFIER 2
#define ISDIGITS(x) ((x >= '0' && x <= '9') ? 1 : 0)

int getop(char s[]);
double atof(char s[]);
double pop();
void push(char c);
void lookupStack();
void dealWithFunc(char s[]);

int main(int argc, char const *argv[])
{
  int type;
  double op, op1, op2;
  char s[MAXLEN] = {0};

  while((type = getop(s)) != EOF)
  {
    printf("Get from getop: %s, type: %d\n", s, type);
    switch(type)
    {
      case NUMBER:
        push(atof(s));
        break;
      case IDENTIFIER:
        dealWithFunc(s);
        break;
      case '+':
        push(pop() + pop());
        break;
      case '-':
        op = pop();
        push(pop() - op);
        break;
      case '*':
        push(pop() * pop());
        break;
      case '/':
        op = pop();
        push(pop() / op);
        break;
      case '%':
        op2 = pop();
        push(fmod(pop(), op2));
        break;
      case '\n':
        printf("\t%.8g\n", pop());
        break;
      default:
        printf("Invalid command found: %s\n", s);
    }

    memset(s, 0, MAXLEN);
  }

  // printf("The end of main\n");
  return 0;
}

void dealWithFunc(char s[])
{
  double op;

  if (strstr(s, "sin"))
  {
    push(sin(pop()));
  }
  else if (strstr(s, "cos"))
  {
    push(cos(pop()));
  }
  else if (strstr(s, "tan"))
  {
    push(tan(pop()));
  }
  else if (strstr(s, "exp"))
  {
    push(exp(pop()));
  }
  else if (strstr(s, "pow"))
  {
    op = pop();
    push(pow(pop(), op));
  }
  else {}
}

double atof(char s[])
{
  // printf("START atof: %s\n", s);
  double value;
  int i, sign, power = 1;

  if (s == NULL)
  {
    return 0;
  }

  sign = s[0] == '-' ? -1 : 1;

  if (s[0] == '+' || s[0] == '-')
  {
    ++i;
  } 

  value = 0;
  for (i = 0; ISDIGITS(s[i]); ++i)
  {
    value = value * 10 + s[i] - '0';
    // printf("atof value: %f\n", value);
  }

  if (s[i] == '.')
  {
    ++i;

    for (; ISDIGITS(s[i]); ++i)
    {
      value = value * 10 + s[i] - '0';
      power *= 10;
    }
  }

  // printf("atof: %f\n", value / power);
  return value / power;
}
