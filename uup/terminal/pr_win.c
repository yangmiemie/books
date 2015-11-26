#include <stdio.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>

void tty_mode(int how);
void err_ret(char* s1, char* s2);
void get_screen_size(int* row, int *col);
void fill_screen(int signum);

char filler = 'A';

int main(int argc, char const *argv[])
{
  int c;
  tty_mode(0);
  fill_screen(0);

  while((c = getchar()) != EOF && (c != 'Q'))
  {
    filler = 'A';
    fill_screen(0);
  }

  tty_mode(1);
  return 0;
}

void fill_screen(int signum)
{
  int row, col;
  int i, j, n;

  get_screen_size(&row, &col);

  n = row * col;
  for (i = 0; i < n; ++i)
  {
    putchar(filler);
  }

  ++filler;
  fflush(stdout);
}

void getout(int signum)
{
  tty_mode(1);
}

void tty_mode(int how)
{
  static struct termios original;
  struct termios term_new;

  if (how == 0)
  {
    tcgetattr(0, &original);

    term_new = original;
    term_new.c_lflag &= ~ECHO;
    term_new.c_lflag &= ~ICANON;

    tcsetattr(0, TCSANOW, &term_new);

    signal(SIGINT, getout);
    signal(SIGQUIT, getout);
    signal(SIGWINCH, fill_screen);
  }
  else
  {
    tcsetattr(0, TCSANOW, &original);
  }
}

void get_screen_size(int* row, int *col)
{
  struct winsize size;
  if (ioctl(0, TIOCGWINSZ,&size) < 0)
  {
    err_ret("TIOCGWINSZ error", "");
  }

  *row = size.ws_row;
  *col = size.ws_col;
}


void err_ret(char* s1, char* s2)
{
  fprintf(stderr, "Error %s ", s1);
  perror(s2);
  exit(1);
}