#include <stdio.h>
#define MAXLEN 64

double stack[MAXLEN];
int stackp = 0;

double pop()
{
  if (stackp < 0)
  {
    printf("The stack is empty\n");
    return 0;
  }

  double value = stack[--stackp];

  // printf("%f has been poped from stack\n", value);
  return value;
}

void push(double d)
{
  if (stackp > MAXLEN)
  {
    printf("The stack is full\n");
    return;
  } 

  // printf("%f has been pushed into stack\n", d);
  stack[stackp++] = d;
}

void printTop()
{
  printf("%f\n", stack[stackp-1]);
}

double getTop()
{
  if (stackp < 1)
  {
    printf("The stack is empty\n");
  }

  return stack[stackp-1];
}

double duplicateTop()
{
  double tmp = pop();

  push(tmp);
  push(tmp);
}

void swapStackTop()
{
  swap(stack + stackp - 2, stack + stackp - 1);
}

void swap(double* d1, double* d2)
{
  int tmp = *d1;
  *d1 = *d2;
  *d2 = tmp;
}

void clear()
{
  stackp = 0;
}

void lookupStack()
{
  int i = 0;
  printf("lookupStack: \n");
  for (i = 0; i < stackp; ++i)
  {
    printf("%f,", stack[i]);
  }
  printf("\n");
}