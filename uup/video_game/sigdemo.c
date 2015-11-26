#include <stdio.h>
#include <signal.h>

#define INPUTLEN 100
void quit_handler(int signum);
void int_handler(int signum);

int main(int argc, char const *argv[])
{
  char input[INPUTLEN];
  int nchars;

  signal(SIGINT, int_handler);
  signal(SIGQUIT, quit_handler);

  do {
    printf("\nType a message\n");
    nchars = read(0, input, INPUTLEN);
    if (nchars == -1)
    {
      perror("read returned an error");
    }
    else
    {
      input[nchars] = '\0';
      printf("You typed: %s\n", input);
    }
  }
  while(strncmp(input, "quit", 4) != 0);
  return 0;
}

void int_handler(int signum)
{
  printf("Received signal %d ... waiting\n", signum);
  sleep(2);
  printf("Leaving int_handler\n");
}

void quit_handler(int signum)
{
  printf("Received signal %d ... waiting\n", signum);
  sleep(3);
  printf("Leaving quit_handler\n");
}