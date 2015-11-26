#include <mqueue.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: mqunlink <name>\n");
		exit(1);
	}

	if ((mq_unlink(argv[1])) != 0)
	{
		perror("Unlink mq error: ");
		exit(1);
	}

	return 0;
}