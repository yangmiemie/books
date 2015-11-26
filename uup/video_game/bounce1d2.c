#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>

int col, row, dir;
int delay = 200;
char* msg = "hello";
char* blank = "     ";
int done = 0;

void move_message();
void enable_kbd_signals();
void on_input(int signum);

int main(int argc, char const *argv[])
{
  int c;

  col = 0;
  row = 10;
  dir = 1;

  initscr();
  clear();
  
  move(row, col);
  addstr(msg);

  signal(SIGIO, on_input);
  enable_kbd_signals();

  while(!done)
  {
    usleep(delay);
    move_message();
  }

  endwin();
  return 0;
}

void enable_kbd_signals()
{
  int flags;
  fcntl(0, F_SETOWN, getpid());
  flags = fcntl(0, F_GETFL);
  fcntl(0, F_SETFL, (flags | O_ASYNC));
}

void on_input(int signum)
{
  int c = getch();
  if (c == 'q')
  {
    done = 1;
  }
  else if (c == 's')
  {
    delay *= 2; 
  }
  else if (c == ' ')
  {
    dir *= -1;
  }
  else if (c == 'f')
  {
    delay /= 2;
  }
}

void move_message()
{
  move(row, col - dir);
  addstr(blank);

  move(row, col);
  addstr(msg);

  move(LINES - 1, 0);
  col += dir;
  // move(row, col + 1);
  refresh();
  if (dir > 0 && col + strlen(msg) >= COLS)
  {
    dir = -1;
  }
  else if (dir < 0 && col <= 0)
  {
    dir = 1;
  }
}