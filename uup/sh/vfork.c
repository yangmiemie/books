#include <sys/types.h>
#include <unistd.h>

int glob = 6;

int main(int argc, char const *argv[])
{
  int var;
  pid_t pid;

  var = 88;

  printf("Before vfork");
  if ((pid = vfork()) < 0)
  {
    perror("vfork error");
  }
  else if (pid == 0)
  {
    glob++;
    var++;
    _exit(0);
  }

  printf("pid = %d, glob = %d, var = %d", getpid(), glob, var);
  _exit(0);
}