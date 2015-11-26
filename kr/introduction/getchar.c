#include <stdio.h>
#define BUFSIZE 4

int main(int argc, char const *argv[])
{
  char c;
  char buf[BUFSIZE] = {0};

  
  // unsigned int i = -1;
  // setvbuf(stdin, buf, _IOLBF, BUFSIZE);
  // setvbuf(stdout, buf, _IOLBF, BUFSIZE);
  // printf("%d\n", EOF);
  while((c = getchar()) != EOF )
  {
    putchar(c);
    // printf("%d", c);   

    // if (c == EOF)
    // {
    //  printf("\n");
    // }
  }

  // printf("%d\n", c);
  // printf("%d\n", i);
  printf("EOF is entered\n");

  return 0;
}