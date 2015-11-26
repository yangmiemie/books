#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

const char question[] = "DO you want another transaction";
#define TRIES 3
#define SLEEPTIME 1

int get_response(const char*);
void set_crmode();
void tty_mode(int how);
int get_ok_char();
void set_unblocked_mode();
void ctrl_c_cntl(int );
int main(int argc, char const *argv[])
{ 
  int response;
  tty_mode(0);
  set_crmode();
  // set_unblocked_mode();
  // signal(SIGINT, ctrl_c_cntl);
  // signal(SIGQUIT, SIG_IGN);
  response = get_response(question);
  printf("response = %d\n", response);
  tty_mode(1);
  return 0;
}

int get_response(const char* question)
{
  int c;
  int tries = TRIES;

  printf("%s (y/n)?", question);
  fflush(stdout);
  while(1)
  {
    // sleep(SLEEPTIME);

    switch((c = get_ok_char()))
    {
      case 'y':
      case 'Y':
        return 0;
      case 'n':
      case 'N':
        return 1;
      default:
        if (--tries == 0)
          return 2;
      // default:
      //  printf("\ncannot understand %c", c);
      //  printf("Please type y or no\n");
    } 
  }
}
void ctrl_c_cntl(int signum)
{
  tty_mode(1);
  exit(1);
}

void set_unblocked_mode()
{
  int mode;
  mode = fcntl(0, F_GETFL);
  mode |= O_NONBLOCK;
  fcntl(0, F_SETFL, mode);
}

int get_ok_char()
{
  int c;
  while((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
    ;

  return c;
}

void set_crmode()
{
  struct termios ttystate;
  tcgetattr(0, &ttystate);
  ttystate.c_lflag &= ~ICANON;
  ttystate.c_lflag &= ~ECHO;
  // ttystate.c_cc[VMIN] = 1;
  ttystate.c_cc[VMIN] = 0;
  ttystate.c_cc[VTIME] = 50;
  tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
  static struct termios original_mode;
  static int mode;

  if (how == 0)
  {
    tcgetattr(0, &original_mode);
    mode = fcntl(0, F_GETFD);
  }
  else
  {
    tcsetattr(0, TCSANOW, &original_mode);
    fcntl(0, F_SETFD, mode);
  }
}