#include  <stdio.h>
#include  <signal.h>

/*
 * number of SIGINTs before quitting
 * This value has to be global so the handler can see it
 */
int quitnum = 10;

main(int ac, char *av[])
{
  void  f(int);     /* declare the handler  */
  int i;

  if ( ac>1 && atoi(av[1])>0 )  /* handle args    */
    quitnum = atoi(av[1]);

  signal( SIGINT, f );    /* install the handler  */

  while(1){     /* main loop    */
    printf("hello\n");
    sleep(1);
  }
}

/*
 * the handler for SIGINT
 *   Print a message and count number of SIGINTS received
 *   Quit when the number reaches quitnum
 */

void f(int signum)
{
  static int hits = 0;    /* hit counter    */
  int    i;     /* loop counter   */

  printf("OUCH");
  hits++;
  for(i=0; i<hits; i++)
    putchar('!');
  putchar('\n');
  if ( hits == quitnum )
    exit(0);
}
