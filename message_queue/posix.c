#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_NAME "/test"
#define MAXSZ 1024
int main() {
	mqd_t mqd;
	struct mq_attr attr;
	unsigned prio = 1;
	char *buf = "test message";

	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAXSZ;
	attr.mq_curmsgs = 0;

	ssize_t len = strlen(buf) + 1;
	mqd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &attr);


	
	if(mqd < 0) {
		perror("mq_open");
		return 1;
	}

	int pid = fork();

	if(pid == 0) {
		if(mq_send(mqd, buf, len, prio) != 0) {
			perror("mq_send");
		}
	}
	else if(pid<0) {
		return 1;
	} else {	
		char buf1[MAXSZ];

		if(mq_receive(mqd, buf1, MAXSZ, &prio) <= 0) {
			perror("mq_recieve");
			return 1;
		}
		printf("message: %s\n", buf1);
		mq_unlink(QUEUE_NAME);
		mq_close(mqd);
	}	

	return 0;
}
