#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

int pid;
void alm_handler(int signum);
time_t modtime(char* file);

int main(int argc, char const *argv[])
{
  pid = atoi(argv[1]);
  int timeout = atoi(argv[2]);
  time_t now;

  // alarm(timeout);
  // signal(SIGALRM, alm_handler);

  while(1)
  {
    time(&now);
    int sleeptime = (modtime("/dev/pts/0") + timeout) - now;
    if (sleeptime > 0)
    {
      sleep(sleeptime);
    }
    else
    {
      break;
    }
  }

  kill(pid, SIGKILL);
  printf("outof while\n");
  exit(0);
}

void alm_handler(int signum)
{
  // kill(pid, SIGKILL);
}

time_t modtime(char* file)
{
  struct stat buf;
  stat(file, &buf);
  return buf.st_mtime;
}