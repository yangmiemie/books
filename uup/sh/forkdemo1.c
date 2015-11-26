int main(int argc, char const *argv[])
{
  int ret_from_fork, mypid;

  mypid = getpid();
  printf("Before: mypid = %d\n", mypid);

  ret_from_fork = fork();

  // sleep(1);

  printf("After: mypid is %d, fork() return %d\n", getpid(), ret_from_fork);
  return 0;
}