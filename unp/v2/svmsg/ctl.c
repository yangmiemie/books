#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>

#define TEXT_SIZE  512
struct msgbuf
{
  long mtype ;
  char mtext[TEXT_SIZE] ;
};

int main(int argc, char const *argv[])
{
  int msqid;
  struct msqid_ds info;
  struct msgbuf buf;

  if ((msqid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT )) < 0)
  {
    perror("msgget error");
    exit(1);
  }

  buf.mtype = 1;
  buf.mtext[0] = 1;
  
  if (msgsnd(msqid, &buf, 1, 0) < 0)
  {
    perror("msgsend error");
    exit(1);
  }

  msgctl(msqid, IPC_STAT, &info);
  printf("read-write: %03o, cbytes = %lu, qnum = %lu, qbytes = %lu\n", info.msg_perm.mode & 0777,
    info.__msg_cbytes, info.msg_qnum, info.msg_qbytes);

  system("ipcs -q");

  msgctl(msqid, IPC_RMID, NULL);
  return 0;
}