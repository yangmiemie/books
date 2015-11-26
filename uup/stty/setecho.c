#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void oops(char *s, int x)
{
  perror(s);
  exit(x);
}

int main(int argc, char *argv[])
{
  struct termios info;

  if (argc != 2)
  {
    oops("tcgetattr", 1);
  }

  if (tcgetattr(1, &info) == -1)
  {
    oops("tcgetattr", 1);
  }

  if (argv[1][0] == 'y')
  {
    info.c_lflag |= ECHO;
  }
  else
  {
    info.c_lflag &= ~ECHO;
  }

  if ((tcsetattr(1, TCSANOW, &info)) == -1)
  {
    oops("tcsetattr", 1);
  }

  return 0;
}