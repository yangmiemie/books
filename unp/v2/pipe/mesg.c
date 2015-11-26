#include "mesg.h"

#include <stdio.h>
#include <unistd.h>

void mesg_send(int fd, MSG msg)
{
	write(fd, msg, msg -> mesg_len + MESGHDRSIZE);
}

int mesg_recv(int fd, MSG msg)
{
	int n;
	int len;

	if ((n = read(fd, msg, MESGHDRSIZE)) == 0)
		return 0;

	if ((len = msg -> mesg_len) > 0)
	{
		if ((n = read(fd, msg -> mesg_data, len)) != len)
		{
			fprintf(stderr, "message data: expected %d, got %d\n", len, n);
			exit(1);
		}

		msg -> mesg_data[n] = '\0';
	}

	return len;
}