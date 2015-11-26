#include <stdio.h>
#include <unistd.h>

int multiple_pipe(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    multiple_pipe(argc, argv);
    return 0;
}

int multiple_pipe(int argc, char *argv[])
{
    int pid, apipe[2], fd;

    if (argc == 1)
    {
        fprintf(stderr, "usage: at least 2 parameters\n");
        exit(1);
    }   

    if (argc == 2)
    {
        execlp(argv[1], argv[1], NULL);
        exit(1);
    }

    if (pipe(apipe) == -1)
    {
        fprintf(stderr, "pipe error%s");
        exit(1);
    }

    if ((pid = fork()) == -1)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)
    {
        close(apipe[1]);
        dup2(apipe[0], 0);
        close(apipe[0]);

        execlp(argv[argc-1], argv[argc-1], NULL);
    }
    else
    {
        close(apipe[0]);
        dup2(apipe[1], 1);
        close(apipe[1]);

        multiple_pipe(argc - 1, argv);
    }

}