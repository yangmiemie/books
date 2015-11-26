#include <stdio.h>
#include <unistd.h>

void err_ret(char* s1, char* s2);

int main(int argc, char const *argv[])
{
    int thepipe[2];
    int pid;

    if (pipe(thepipe) == -1)
        err_ret("pipe error", "");

    if ((pid = fork()) == -1)
        err_ret("fork error", "");
    else if (pid == 0)
    {
        close(thepipe[1]);
        dup2(thepipe[0], 0);
        close(thepipe[0]);

        // execlp(argv[1], argv[1], NULL);

        // execlp(argv[1], argv[1], NULL);
    }
    else
    {
        close(thepipe[0]);
        dup2(thepipe[1], 1);
        close(thepipe[1]);

        printf("This is the pipe.c file\n");
        fflush(stdout);
        // close(1);
        wait(NULL);
    }


    return 0;
}

void err_ret(char* s1, char* s2)
{   
    fprintf(stderr, "Error: %s\n", s1);
    perror(s2);
    exit(1);    
}