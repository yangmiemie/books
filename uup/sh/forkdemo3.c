int main(int argc, char const *argv[])
{
  int fork_rv;

  printf("Before: my pid is %d\n", getpid());

  fork_rv = fork();

  if (fork_rv == -1)
    perror("fork");
  else if (fork_rv == 0)
  {
    printf("I am child. my pid = %d\n", getpid());
  }
  else
  {
    printf("I am parent. my child pid = %d\n", fork_rv);
  }
  return 0;
}