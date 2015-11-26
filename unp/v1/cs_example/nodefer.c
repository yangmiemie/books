#include <signal.h>
#include <stdio.h>

#define NUMBER 5
typedef void (*sighandler_t)(int);
typedef void (*sigaction_t)(int, siginfo_t*, void*);

sighandler_t Signal(int signo, sighandler_t func)
{
  struct sigaction act, oact;
  sigset_t set;

  // act.sa_sigaction = func;
  act.sa_handler = func;
  sigemptyset(&set);
  act.sa_mask = set;
  act.sa_flags = 0;
  act.sa_flags |= SA_NODEFER;  
  // act.sa_flags |= SA_SIGINFO;  
  // act.sa_flags |= SA_RESTART;

  if (sigaction(signo, &act, &oact) < 0)
    return SIG_ERR;

  return oact.sa_handler;
}

// void sig_chld(int signo, siginfo_t* info, void* context)
void sig_chld(int signo)
{
  int stat;
  pid_t pid;

  // printf("received SIGCHLD, code = %d, value = %d\n", info -> si_signo, info -> si_value.sival_int);
  printf("received SIGCHLD, signo = %d\n", signo);
  sleep(10);
  printf("done with SIGCHLD\n");
  // pid = wait(&stat);
  // printf("child %d teminated\n", pid);
  
  return;
}

int main(int argc, char const *argv[])
{
  int i;
  pid_t pid;
  union sigval val;

  Signal(SIGCHLD, sig_chld);

  pause();
  // for (i = 0; i < NUMBER; ++i)
  // {
  //   val.sival_int = i;
  // raise(SIGINT);
  // sleep(1);
  // raise(SIGINT);
  // sleep(1);
  // raise(SIGINT);
  // sleep(1);
  // raise(SIGINT);
  // sleep(1);
  // raise(SIGINT);
  // sleep(1);
  // raise(SIGINT);
  //   // sigqueue(pid, SIGCHLD, val);
  // }

  return 0;
}