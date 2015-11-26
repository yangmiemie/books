#include <stdio.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
  int fd;
  char line[100];

  fgets(line, 100, stdin); printf("%s", line);  
  fgets(line, 100, stdin); printf("%s", line);  
  fgets(line, 100, stdin); printf("%s", line);  

  close(0);
  fd = open("/etc/passwd", O_RDONLY);

  fgets(line, 100, stdin); printf("%s", line);  
  fgets(line, 100, stdin); printf("%s", line);  
  fgets(line, 100, stdin); printf("%s", line);  

  return 0;
}