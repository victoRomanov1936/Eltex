#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSGSZ 128

struct msgbuf{
		long mtype;
		char mtext[MSGSZ];
	};

int main() {
	struct msgbuf read_msg;
	struct msgbuf write_msg = {1, "Test message"};
	int lenght_msg = strlen(write_msg.mtext) + 1; 	
	key_t key = ftok("systemV.c", 0X45);
	int msqid = msgget(key, IPC_CREAT | 0400 | 0200);

	if(msqid < 0) {
		perror("msgget");
		return 1;
	}
	 
	int pid = fork();
	
	if(pid == 0) {
		if(msgsnd(msqid, &write_msg, lenght_msg, IPC_NOWAIT) < 0){
			perror("msgsnd");
		}
	} else if(pid < 0) {
		msgctl(msqid, IPC_RMID, NULL);
		return 1;		
	} else {
		if(msgrcv(msqid, &read_msg, MSGSZ, 1 , 0) < 0) {
			perror("msgrcv");
		}
		msgctl(msqid, IPC_RMID, NULL);
		printf("%s", read_msg.mtext);
	}
	
	return 0;
}
