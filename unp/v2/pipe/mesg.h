#define MAXMESGDATA 512
#define MESGHDRSIZE 8

typedef struct mesg* MSG;

struct mesg {
	int mesg_len;
	int mesg_type;
	char mesg_data[MAXMESGDATA];
};

void mesg_send(int, MSG);
int mesg_recv(int, MSG);