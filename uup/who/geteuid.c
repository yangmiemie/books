#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>

int main(int argc, char const *argv[])
{
  uid_t euid;
  struct passwd* ptr;

  euid = geteuid();
  ptr = getpwuid(euid);

  if (ptr != NULL)
  {
    printf("%s\n", ptr->pw_name);
  }

  return 0;
}