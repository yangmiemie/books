#include <stdio.h>
#include <unistd.h>

#define MAX 64

void err_ret(char* s1, char* s2);

int main(int argc, char const *argv[])
{
	int thepipe[2];
	int pid;
	int n;
	char buffer[MAX];

	if (pipe(thepipe) == -1)
		err_ret("pipe error", "");

	if ((pid = fork()) == -1)
		err_ret("fork error", "");
	else if (pid == 0)
	{
		close(thepipe[1]);
		dup2(thepipe[0], 0);
		close(thepipe[0]);

		sleep(3);

		while ((n = read(0, buffer, MAX)) > 0)
		{
			printf("After sleep 3 seconds, Read %d bytes:\n", n);
			write(1, buffer, n);
		}
	}
	else
	{
		close(thepipe[0]);
		dup2(thepipe[1], 1);
		close(thepipe[1]);

		printf("This is the pipe.c file\n");
		sleep(1);
		printf("A message after 1 second\n");
		fflush(stdout);
		close(1);
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