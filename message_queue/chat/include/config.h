#define KEY 100
#define MSGSZ 1024
#define ESCAPE_CODE "\033[2J\033[0;0f"

struct msgbuf{
		long mtype;
		char mtext[MSGSZ];
	};
