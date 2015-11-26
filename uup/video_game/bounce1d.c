#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>

int col, row, dir;
int delay = 200;
char* msg = "hello";
char* blank = "     ";

void set_ticker(int delay);
void move_message(int signum);

int main(int argc, char const *argv[])
{
  int c;

  col = 0;
  row = 10;
  dir = 1;

  initscr();
  clear();
  // move(row, col);
  // addstr(msg);

  signal(SIGALRM, move_message);
  set_ticker(delay);

  while(1)
  {
    c = getch();
    if (c == 'q')
    {
      break;
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

    set_ticker(delay);
  }

  endwin();
  return 0;
}

void set_ticker(int delay)
{
  struct itimerval new_timer;
  int sec, usec;
  sec = delay / 1000;
  usec = (delay % 1000) * 1000;

  new_timer.it_interval.tv_sec = sec;
  new_timer.it_interval.tv_usec = usec;
  new_timer.it_value.tv_sec = sec;
  new_timer.it_value.tv_usec = usec;

  setitimer(ITIMER_REAL, &new_timer, NULL);
}

void move_message(int signum)
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

  set_ticker(delay);
}