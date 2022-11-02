#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "include/config.h"

int main() {
	struct msgbuf write_msg;
	key_t key = KEY;
	char buf_msg[MSGSZ - 20];
	char buf[MSGSZ];
	int msqid = msgget(key, 0666);
	
	if(msqid < 0) {
		perror("msgget");
		return -1;
	}
	int pid = getpid();
	
	
	write_msg.mtype = 1;

	printf(ESCAPE_CODE);
	printf("CLIENT\n%d\n\n", pid);
	for(;;) {
		printf("Send msg:\n");
		
		fgets(buf_msg, MSGSZ - 20, stdin);
		sprintf(buf,"%d: ",pid);
		strcat(buf,buf_msg);
		strcpy(write_msg.mtext,buf);
		if(msgsnd(msqid, &write_msg, MSGSZ, 0) < 0){
			perror("msgsnd");
		}
		
		printf(ESCAPE_CODE);
		printf("CLIENT\n%d\n\n",pid);
	}
	return 0;
}
