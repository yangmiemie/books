#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <aio.h>

int col, row, dir;
int delay = 200;
char* msg = "hello";
char* blank = "     ";
int done = 0;
struct aiocb kbcbuf;

void set_ticker(int delay);
void move_message(int signum);
void setup_aio_buffer();
void enable_kbd_signals();
void on_input(int signum);

int main(int argc, char const *argv[])
{
  col = 0;
  row = 10;
  dir = 1;

  initscr();
  clear();

  signal(SIGALRM, move_message);
  set_ticker(delay);
  // enable_kbd_signals();

  signal(SIGIO, on_input);
  setup_aio_buffer();
  aio_read(&kbcbuf);

  while(! done)
  { 
    pause();
  }

  endwin();
  return 0;
}

void setup_aio_buffer()
{
  static char input[1];

  kbcbuf.aio_fildes = 0;
  kbcbuf.aio_buf = input;
  kbcbuf.aio_nbytes = 1;
  kbcbuf.aio_offset = 0;

  kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  kbcbuf.aio_sigevent.sigev_signo = SIGIO;
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
  int c;
  char *ptr = (char*)kbcbuf.aio_buf;

  if (aio_error(&kbcbuf) != 0)
  {
    perror("aio reading failed");
  }
  else
  {
    if (aio_return(&kbcbuf) == 1)
    {
      int c = *ptr;
      if (c == 'Q')
      {
        done = 1;
      }
      else if (c == 'f')
      {
        delay /= 2;
      }
      else if (c == 's')
      {
        delay *= 2;
      }
      else if (c == ' ')
      {
        dir *= -1;
      }
    }
  }

  aio_read(&kbcbuf);
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
  move(row, col);
  addstr(blank);

  col += dir;
  move(row, col);
  addstr(msg);

  move(LINES - 1, 0);
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