#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define MSGSIZE 256
#define  NMSG 16

struct shmstruct {
	sem_t nempty;
	sem_t nstored;
	sem_t mutex;
	int nput;
	sem_t noverflowmutex;
	int noverflow;

	int msgoff[NMSG];
	char msgdata[MSGSIZE * NMSG];
};