#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

void tty_mode(int how);

int main(int argc, char const *argv[])
{
  int c;
  struct termios info;

  tty_mode(0);
  tcgetattr(0, &info);
  // info.c_lflag &= ~ECHO;
  info.c_lflag &= ~ICANON;
  info.c_lflag &= ~ISIG;
  info.c_cc[VMIN] = 5;
  info.c_cc[VTIME] = 50;
  tcsetattr(0, TCSANOW, &info);

  while(((c = getchar()) != EOF))
  {
    if (c == 'z')
    {
      c = 'a';
    }
    else if (islower(c))
    {
      ++c;
    }
    else if (c == 'Q')
    {
      tty_mode(1);
      return 0;
    }
    else
    {
      continue;
    }

    putchar(c);
  }

  tty_mode(1);
  return 0;
}

void tty_mode(int how)
{
  static struct termios original_mode;
  if (how == 0)
  {
    tcgetattr(0, &original_mode);
  }
  else
  {
    tcsetattr(0, TCSANOW, &original_mode);
  }
}