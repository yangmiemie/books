#include <mqueue.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  int len, prio;
  char *buffer;
  mqd_t mqd;

  len = atoi(argv[2]);
  prio = atoi(argv[3]);

  if ((mqd = mq_open(argv[1], O_WRONLY)) == -1)
  {
    perror("mq open error: ");
    exit(1);
  }

  printf("mqd = %d\n", mqd);

  buffer = malloc(sizeof(buffer[0]) * len);
  memset(buffer, 0, len * sizeof(buffer[0]));
  memset(buffer, 'a', len * sizeof(buffer[0]) - 1);

  if (mq_send(mqd, buffer, len, prio) != 0)
  {
    perror("mq send error");
    exit(1);
  }

  return 0;
}