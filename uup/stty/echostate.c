#include <termios.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  struct termios info;

  if (tcgetattr(0, &info) == -1)
  {
    perror("tcgetattr");
    exit(1);
  }

  if (info.c_lflag & ECHO)
  {
    printf("echo is on\n");
  }
  else
  {
    printf("echo is off\n");
  }
  return 0;
}