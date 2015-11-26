#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 1024

void run_command(int argc, char* argv[], int apipe[2]);
void split_data(int apipe[2]);
void put_data(int fd, char *buf, int amt);
int main(int argc, char *argv[])
{
  int pid;
  int apipe[2];

  pipe(apipe);

  pid = fork();
  if (pid == 0)
    run_command(argc, argv, apipe);
  else
    split_data(apipe);

  return 0;
}

void run_command(int argc, char* argv[], int apipe[2])
{
  int i;
  char buf[BUFSIZE];

  close(apipe[1]);
  dup2(apipe[0], 0);
  close(apipe[0]);

  for(i = 1; i < argc; i++)
    argv[i-1] = argv[i];
  argv[argc-1] = NULL;

  while ((fgets(buf, BUFSIZE, stdin)) != NULL)
  {
    printf("%s", buf);
  }

  // printf("End of while fgets\n");
  // printf("argv[0] = %s, argc = %d\n", argv[0], argc);
  // execvp(argv[0], argv);
  // execlp(argv[1], argv[1], NULL);
}

void split_data(int apipe[2])
{
  char buf[BUFSIZE];
  int n;
  
  close(apipe[0]);

  while((n = read(0, buf, BUFSIZE)) != 0)
  {
    // put_data(1, buf, n);
    // put_data(apipe[1], buf, n);
    write(1, buf, n);
    write(apipe[1], buf, n);
  }

  // close(apipe[1]);

  wait(NULL);
}

void
put_data(int fd, char *buf, int amt)
{
  int amt_out;
  int total_put = 0;

  while( total_put < amt )
  {
    amt_out = write(fd,buf+total_put,(amt-total_put));
    // if ( amt_out == -1 )
    //  oops("writing out data",5);
    total_put += amt_out;
  }
}
