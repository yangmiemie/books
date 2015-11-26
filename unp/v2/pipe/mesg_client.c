#include "mesg.h"

#include <stdio.h>

void client(int readfd, int writefd)
{
	struct mesg msg;
	int len;
	int n;

	while ((len = read(0, msg.mesg_data, MAXMESGDATA)) > 0)
	{
		if (msg.mesg_data[len-1] == '\n')
		{
			msg.mesg_data[len-1] = '\0';
		}

		msg.mesg_len = len - 1;
		msg.mesg_type = 1;

		mesg_send(writefd, &msg);

		while ((n = mesg_recv(readfd, &msg)) > 0)
			write(0, msg.mesg_data, n);

		printf("\n");
	}
}