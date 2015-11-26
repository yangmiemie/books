#include "mesg.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
extern int errno;

void server(int readfd, int writefd)
{
	struct mesg msg;
	int fd;
	int n;

	while ((n = mesg_recv(readfd, &msg)) > 0)
	{
		if ((fd = open(msg.mesg_data, O_RDONLY)) == -1)
		{
			sprintf(msg.mesg_data + n, " cannot open, %s", strerror(errno));
			msg.mesg_len = strlen(msg.mesg_data);
			mesg_send(writefd, &msg);
		}
		else
		{
			while ((n = read(fd, msg.mesg_data, MAXMESGDATA)) > 0)
			{
				msg.mesg_len = n;
				mesg_send(writefd, &msg);
			}
		}

		msg.mesg_len = 0;
		mesg_send(writefd, &msg);

		close(fd);
	}
}