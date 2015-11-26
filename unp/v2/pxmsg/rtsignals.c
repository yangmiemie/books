#include <signal.h>
#include <stdio.h>

typedef void (*sighandler_t)(int, siginfo_t*, void*);

sighandler_t signal_rt(int signo, sighandler_t func, sigset_t *mask);
void sig_rt(int, siginfo_t*, void*);

int main(int argc, char const *argv[])
{
  sigset_t newset;
  int pid;
  int i, j;
  union sigval val;

  sigemptyset(&newset);
  sigaddset(&newset, SIGRTMAX);
  sigaddset(&newset, SIGRTMAX - 1);
  sigaddset(&newset, SIGRTMAX - 2);

  if ((pid = fork()) == 0)
  {
    sigprocmask(SIG_BLOCK, &newset, NULL);
    signal_rt(SIGRTMAX, sig_rt, &newset);
    signal_rt(SIGRTMAX - 1, sig_rt, &newset);
    signal_rt(SIGRTMAX - 2, sig_rt, &newset);
    
    sleep(6);
    sigprocmask(SIG_UNBLOCK, &newset, NULL);
    sleep(3);
    exit(1);
  }

  sleep(3);
  for (i = 0; i < 3; ++i)
  {
    for (j = SIGRTMAX; j >= SIGRTMAX - 2; --j)
    {
      val.sival_int = i;
      sigqueue(pid, j, val);
    }
  }

  return 0;
}

sighandler_t signal_rt(int signo, sighandler_t func, sigset_t *mask)
{
  struct sigaction act, oact;
  act.sa_sigaction = func;
  act.sa_mask = *mask;
  act.sa_flags = SA_SIGINFO;

  if (sigaction(signo, &act, &oact) == -1)
  {
    return oact.sa_sigaction;
  }

  return act.sa_sigaction;
}

void sig_rt(int signo, siginfo_t* info, void* context)
{
  printf("received signal %d, code = %d, ival = %d\n", signo, info -> si_code, info -> si_value.sival_int);
}