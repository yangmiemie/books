#include <curses.h>
#include <signal.h>

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

void wakeup(int signum);
#define WAKEUP

int main(int argc, char const *argv[])
{
  char *message = "Hello";
  char *blank = "    ";
  int dir = 1;
  int pos = LEFTEDGE;

  initscr();
  clear();

  while(1)
  {
    move(ROW, pos);
    addstr(message);
    move(LINES-1, COLS-1);
    refresh();
    // sleep(1);
    signal(SIGALRM, wakeup);
    alarm(1);
    pause();
    move(ROW, pos);
    addstr(blank);
    pos += dir;
    if (pos >= RIGHTEDGE)
    {
      dir = -1;
    }
    if (pos <= LEFTEDGE)
    {
      dir = 1;
    }
  }

  return 0;
}

void wakeup(int signum)
{
#ifndef WAKEUP
  printf("Wakeup\n");
#endif
}