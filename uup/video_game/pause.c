#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

void deal()
{

   printf("信号干扰！\n");

}


void main()
{
    printf("进程执行！\n");
    signal(SIGINT,deal);
    pause();
    printf("进程结束！\n");

}