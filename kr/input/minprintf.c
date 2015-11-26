#include <stdarg.h>

void minprintf(char* fmt, ...);

int main(int argc, char const *argv[])
{
  char *s = "abcdefghijk";
  int i = 100;
  double d = 0.32;

  minprintf("xyz %f, %d, %s\n", d, i, s);
  return 0;
}

void minprintf(char* fmt, ...)
{
  va_list ap;
  char *p, *sval;
  double dval;
  int ival;

  va_start(ap, fmt);

  for (p = fmt; *p; p++)
  {
    if (*p != '%')
    {
      putchar(*p);
      continue;
    }

    switch(*++p)
    {
      case 's':
        sval = va_arg(ap, char*);
        printf("%s", sval);
      break;
      case 'd':
        ival = va_arg(ap, int);
        printf("%d", ival);
      break;
      case 'f':
        dval = va_arg(ap, double);
        printf("%f", dval);
      break;
      default:
        putchar(*p);
        break;
    }
  }

  va_end(ap); 
}