#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define MAX(a, b) ((a > b) ? (a) : (b))

void sig_segv(int signo)
{
	printf("catch SIGSEGV\n");
	exit(1);
}
void sig_bus(int signo)
{
	printf("catch SIGBUS\n");
	exit(1);
}

int main(int argc, char const *argv[])
{
	int mmapsize, pagesize, filesize;
	int fd, i, j;
	char *ptr;
	int pagenums;

	if (argc != 4)
	{
		fprintf(stderr, "usage: test1 <pathname> <filesize> <mmapsize>\n");
	}

 	signal(SIGSEGV, sig_segv);
 	signal(SIGBUS, sig_bus);

	if ((fd = open(argv[1], O_RDWR | O_CREAT, 0666)) < 0)
	{
		perror("open error");
		exit(1);
	}

	filesize = atoi(argv[2]);
	mmapsize = atoi(argv[3]);

	printf("filesize = %d, mmapsize = %d\n", filesize, mmapsize);
	lseek(fd, filesize - 1, SEEK_SET);
	write(fd, "A", 1);

	ptr = mmap(NULL, mmapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	pagesize = sysconf(_SC_PAGESIZE);
	printf("Page Size = %d\n", pagesize);

	pagenums = filesize / pagesize + 1;
	printf("Page Number = %d\n", pagenums);

	for (i = 0, j = 0; j < pagenums * 10; i += pagesize, j++)
	{
		printf("ptr[%d] = %d\n", i, ptr[i]);
		ptr[i] = 'A';
		printf("ptr[%d] = %d\n", i + pagesize - 1, ptr[i + pagesize - 1]);
		ptr[i + pagesize - 1] = 'A';
	}

	// for (i = 0; i < MAX(mmapsize, filesize) * 2; ++i)
	// {
	// 	ptr[i] = 'A';
	// }
	// printf("\n");
	// printf("ptr[%d] = %d\n", i * 3, ptr[i * 3]);

	// if (unlink(argv[1]) < 0)
	// {
	// 	perror("unlink error");
	// 	exit(1);
	// }

	return 0;
}