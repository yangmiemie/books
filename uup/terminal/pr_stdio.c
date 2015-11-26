#include <stdio.h>

void pr_stdio(const char*, FILE*);

int main(int argc, char const *argv[])
{
  FILE *fp1, *fp2;
  // getchar();
  // pr_stdio("stdout", stdout);
  // pr_stdio("stdin", stdin);
  // pr_stdio("stderr", stderr);
  // fprintf(stderr, "This is a errro message");

  fp1 = fopen("./err.out", "r");
  fp2 = fopen("./err.out", "w");

  fgetc(fp1);
  fprintf(fp2, "abc");

  pr_stdio("./err.out read", fp1);
  pr_stdio("./err.out write", fp2);
  return 0;
}

void pr_stdio(const char* name, FILE *fp)
{
  // printf("stream = %s, ", name);

  if (fp->_IO_file_flags & _IO_UNBUFFERED)
  {
    printf("unbuffered");
  }
  else if (fp->_IO_file_flags & _IO_LINE_BUF)
  {
    printf("line buffered");
  }
  else
  {
    printf("fully buffered");
  }

  printf(", buffer size = %d\n", fp->_IO_buf_end - fp->_IO_buf_base);
}