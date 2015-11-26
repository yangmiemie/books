#include <unistd.h>
#include <stdio.h>

void f1();
void f2();
void f3();
void f4();

int main(int argc, char const *argv[])
{
  f1();
  exit(0);
}

void f1()
{
  pid_t pid;

  pid = vfork();

  if (pid == 0)
  {
    f3();
    exit(0);
  }
}

void f2()
{
  printf("This is f2\n");
}

void f3()
{
  printf("This is f3\n");
  f4();
  return;
}

void f4()
{
  printf("This is f4\n");
  return;
}