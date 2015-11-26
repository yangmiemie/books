/*************************************************************************
> File Name: demo_sigio.c
> Author: liuxingen
> Mail: liuxingen@nsfocus.com 
> Created Time: 2014年08月14日 星期四 21时32分03秒
************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include<signal.h>
#include<fcntl.h>

static int g_fd;

static void sigio_handler(int signum)
{
    char buf[8] = {0};

    while (read(g_fd, buf, 7) > 0)
    {
        printf("sigio recv:%s\n", buf);
        // fprintf(stderr, "read error:%s\n", strerror(errno));
    }
    // else
    // {
    //     printf("sigio recv:%s\n", buf);
    // }
}
int main(int argc, char *argv[])
{
    struct sigaction act;
    int flags, i = 1, fds[2];
    pid_t pid;

    if(pipe(fds) < 0)
    {
        fprintf(stderr, "pipe error:%s\n", strerror(errno));
        return 1;
    }
    if((pid = fork()) > 0)
    {
        close(fds[1]);
        dup2(fds[0], g_fd);

        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_RESTART;
        act.sa_handler = sigio_handler;
        if(sigaction(SIGIO, &act, NULL) == -1)
        {
            fprintf(stderr, "sigaction error:%s\n", strerror(errno));
            return 1;
        }

        if(fcntl(g_fd, F_SETOWN, getpid()) == -1)
        {
            fprintf(stderr, "fcntl F_SETOWN error:%s\n", strerror(errno));
            return 1;
        }

        flags = fcntl(g_fd, F_GETFL);
        if(fcntl(g_fd, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1)
        {
            fprintf(stderr, "fcntl F_GETFL error:%s\n", strerror(errno));
            return 1;
        }
        while(1)
        {
            sleep(10);
        }
    }else
    {
        char buf[20] = {0};
        close(fds[0]);
        for(i = 0; i < 3; i++)
        {
            snprintf(buf, 20, "this is loop %d", i);
            write(fds[1], buf, strlen(buf));
            printf("loop %d\n", i);
            sleep(3);
        }
    }

    return 0;
}