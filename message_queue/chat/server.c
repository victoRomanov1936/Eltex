#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "include/config.h"

int main() {
	struct msgbuf read_msg;
	key_t key = KEY;
	int msqid = msgget(key, IPC_CREAT | 0666 );

	if(msqid < 0) {
		perror("msgget");
		return -1;
	}

	printf(ESCAPE_CODE);
	printf("SERVER\n\n");
	for(;;) {

		if(msgrcv(msqid, &read_msg, MSGSZ, 1 , 0) < 0) {
			perror("msgrcv");
		} else {
			printf("%s", read_msg.mtext);
		}
	}
	msgctl(msqid, IPC_RMID, NULL);
	return 0;
}

