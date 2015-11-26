#include <stdio.h>

int main(int argc, char const *argv[])
{
  int i;

  printf("The number of args: %d, Args are: \n", argc);

  for (i = 0; i < argc; ++i)
    printf("args[%d]: %s\n", i, argv[i]);

  fflush(stdout);

  fprintf(stderr, "This is message sent to stderr\n");

  return 0;
}