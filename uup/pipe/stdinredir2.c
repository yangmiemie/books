#include <stdio.h>
#include <fcntl.h>

// #define CLOSE_UP

int main(int argc, char const *argv[])
{
  int fd;
  char line[100];

  // fgets(line, 100, stdin); printf("%s", line); 
  // fgets(line, 100, stdin); printf("%s", line); 
  // fgets(line, 100, stdin); printf("%s", line); 

  fd = open("/etc/passwd", O_RDONLY);

  #ifdef CLOSE_UP
  close(0);
  dup(fd);
  # else
  dup2(fd, 0);
  # endif

  close(fd);

  fgets(line, 100, stdin); printf("%s", line);  
  fgets(line, 100, stdin); printf("%s", line);  
  fgets(line, 100, stdin); printf("%s", line);  

  return 0;
}