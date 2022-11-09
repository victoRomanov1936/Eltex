#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVERS_NUM 5
#define FIRST_PORT 4444
#define ADRESS "127.0.0.1"
#define MSGSZ 256

struct Msgbuf{
	long mtype;
	int client;
};

typedef struct Thread{
	int msqid, port;
}ThreadArg;


void* ThreadServerMain(void *argv) {
	ThreadArg* arg = (ThreadArg*) argv;
	struct Msgbuf write_msg = {1};
	int lenght_msg = sizeof(int);
	int client;
	
	struct sockaddr_in serv_adr = {0}, client_adr;
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(arg->port);
	inet_pton(AF_INET, "127.0.0.1", &serv_adr.sin_addr);
	
	int server = socket(AF_INET, SOCK_STREAM, 0);
	
	if(server == -1) {
		perror("server_fd");
		pthread_exit(NULL);
	}

	if(bind(server,(struct sockaddr*) &serv_adr, sizeof serv_adr) == -1) {
		perror("bind");
		pthread_exit(NULL);
	}

	if(listen(server, 5) == -1) {
		perror("listen");
		pthread_exit(NULL);
	}
	
	socklen_t adrlen =  sizeof serv_adr;

	for(;;) {
		client = accept(server, (struct sockaddr*) &client_adr, &adrlen);

		if(client == -1) {
			perror("accept");
			pthread_exit(NULL);
		}

		write_msg.client = client;
		
		if(msgsnd(arg->msqid, &write_msg, lenght_msg, IPC_NOWAIT) < 0){
			perror("msgsnd");
			pthread_exit(NULL);
		}
	}

	sleep(1);
	close(client);
	close(server);
	pthread_exit(NULL);
}

void* ThreadServer(void *argv) {
	ThreadArg* arg = (ThreadArg*) argv;
	struct Msgbuf read_msg;
	time_t now;
	
	struct sockaddr_in serv_adr = {0};
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(arg->port);
	inet_pton(AF_INET, "127.0.0.1", &serv_adr.sin_addr);
	
	int server = socket(AF_INET, SOCK_STREAM, 0);
	
	if(server == -1) {
		perror("server_fd");
		pthread_exit(NULL);
	}

	if(bind(server,(struct sockaddr*) &serv_adr, sizeof serv_adr) == -1) {
		perror("bind");
		pthread_exit(NULL);
	}

	for(;;) {
		
		if(msgrcv(arg->msqid, &read_msg, sizeof(int), 1 , 0) < 0) {
			perror("msgrcv");
			pthread_exit(NULL);
		}

		time(&now);
		
		if(send(read_msg.client, &now, sizeof(time_t), 0) == -1) {
			perror("send");
			pthread_exit(NULL);
		}

		close(read_msg.client);
	}
	
	close(server);
	pthread_exit(NULL);
}

int main() {
	pthread_t threads_servers[SERVERS_NUM + 1];
	key_t key = ftok("server.c", 0X45);
	int *s;
	int msqid = msgget(key, IPC_CREAT | 0400 | 0200);
	ThreadArg ta[SERVERS_NUM + 1];

	for(int i = 0; i <= SERVERS_NUM; i++) {
		ta[i].msqid = msqid;
		ta[i].port = FIRST_PORT + i;
	}
	
	printf("msgq:%d\n", msqid);
	
	pthread_create(&threads_servers[0], NULL, ThreadServerMain, &ta[0]);
	
	for(int i = 1; i < SERVERS_NUM + 1; i++) {
		pthread_create(&threads_servers[i], NULL, ThreadServer, &ta[i]);
	}
	
	for(int i = 0; i < SERVERS_NUM + 1; i++) {
		pthread_join(threads_servers[i], (void **) &s);
	}
	msgctl(msqid, IPC_RMID, NULL);
	return 0;
}
