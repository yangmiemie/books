#include <stdio.h>
#include <termios.h>
#include <errno.h>

#define MAXLINELEN 512
#define PAGELEN 24

void do_more(FILE*);
int see_more(FILE*);
void tty_mode(int mode);
void  set_tty_mode();

int main(int argc, char *argv[])
{
  FILE *fp;
  char *pFileName;

  tty_mode(0);

  set_tty_mode();
  if (argc == 1)
  {
    do_more(stdin);
  }
  else
  {
    while(--argc > 0)
    {
      pFileName = *++argv;
      if ((fp = fopen(pFileName, "r")) > 0)
      {
        do_more(fp);        
      }
      else
      {
        fprintf(stderr, "Open file %s error, errno = %d\n", pFileName, errno);
      }
    }
  }

  tty_mode(1);
  return 0;
}

void set_tty_mode()
{
  struct termios buf;
  tcgetattr(0, &buf);
  buf.c_lflag &= ~ICANON;
  buf.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &buf);
}


void do_more(FILE* fp)
{
  char line[MAXLINELEN];
  int lineNum, reply;

  FILE* fp_tty;
  fp_tty = fopen("/dev/tty", "r");
  if (fp_tty == NULL)
  {
    exit(1);
  }

  lineNum = 0;
  while((fgets(line, MAXLINELEN, fp)) != NULL)
  {
    if (lineNum == PAGELEN - 1)
    {
      reply = see_more(fp_tty);

      if (reply == -1)
      {
        return;
      }

      lineNum -= reply;
    }
    
    fputs(line, stdout);
    ++lineNum;
  }
}

int see_more(FILE* cmd)
{
  int c, lines;

  while ((c = getc(cmd)) != EOF)
  {
    if (c == '\n')
    {
      lines = 1;
      break;
    }
    else if (c == 'q')
    {
      lines = -1;
      break;
    }
    else if (c == ' ')
    {
      lines = PAGELEN;
      break;
    }
    else 
    {

    }
    // ungetc(c, cmd);
  }

  return lines;
}

void tty_mode(int mode)
{
  static struct termios buf;
  if (mode == 0)
  {
    tcgetattr(0, &buf);
  }
  else
  {
    tcsetattr(0, TCSANOW, &buf);
  }
}