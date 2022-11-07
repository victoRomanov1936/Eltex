#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>   
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVERS_NUM 100
#define FIRST_PORT 30000
#define ADRESS "127.0.0.1"

typedef struct thread_arg{
	pthread_mutex_t mutex;
	int status;
	short port;
} arg;

void *ThreadServer(void *args) {
	arg* mutex = (arg*) args;
	time_t now;
	
	struct sockaddr_in serv_adr = {0}, client_adr;
	
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(mutex->port);
	inet_pton(AF_INET, ADRESS, &serv_adr.sin_addr);
	
	int server = socket(AF_INET, SOCK_STREAM, 0);
	
	if(server == -1) {
		perror("server_fd");
		pthread_exit(NULL);
	}

	if(bind(server,(struct sockaddr*) &serv_adr, sizeof serv_adr) == -1) {
		perror("bind");
		pthread_exit(NULL);
	}

	if(listen(server, 1) == -1) {
		perror("listen");
		pthread_exit(NULL);
	}

	pthread_mutex_lock(&(mutex->mutex));  
		mutex->status = 0;
	pthread_mutex_unlock(&(mutex->mutex));

	int client;
	char buf[256];
	socklen_t adrlen =  sizeof serv_adr;
	
	for(;;) {
		
		client = accept(server, (struct sockaddr*) &client_adr, &adrlen);

		if(client == -1) {
			perror("accept");
			pthread_exit(NULL);
		}
		
		time(&now);
		sprintf(buf,"%s", ctime(&now));
		
		if(send(client, buf, 256, 0) == -1) {
			perror("send");
			pthread_exit(NULL);
		}
		sleep(1);
		
		pthread_mutex_lock(&(mutex->mutex));  
			mutex->status = 0;
		pthread_mutex_unlock(&(mutex->mutex));
		
		close(client); 
	}
	
	close(server);
	pthread_exit(NULL);
}

void *ThreadServerMain(void *args) {
	arg* mutex = (arg*) args;
	
	struct sockaddr_in serv_adr = {0}, client_adr;
	
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(mutex[0].port);
	inet_pton(AF_INET, ADRESS, &serv_adr.sin_addr);
	
	int server = socket(AF_INET, SOCK_STREAM, 0);
	
	if(server == -1) {
		perror("server_fd");
		exit(EXIT_FAILURE);
	}

	if(bind(server,(struct sockaddr*) &serv_adr, sizeof serv_adr) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if(listen(server, 5) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int client;
	socklen_t adrlen =  sizeof serv_adr;
	short port_serv = 0;
	
	for(;;) {
		
		client = accept(server, (struct sockaddr*) &client_adr, &adrlen);
		
		pthread_mutex_lock(&(mutex[0].mutex));
		
			for (int i = 0; i < SERVERS_NUM; i ++) {
				if(mutex[i + 1].status == 0) {
					port_serv = mutex[i + 1].port;
					if(send(client,(short*) &port_serv , sizeof(short), 0) == -1) {
						perror("send");
						exit(EXIT_FAILURE);
					}
					mutex[i + 1].status = 1;
					i = SERVERS_NUM + 1;
				}
			}
			
		pthread_mutex_unlock(&(mutex[0].mutex));
		close(client); 
	}
	close(server);
	pthread_exit(NULL);
}

int main() {
	arg param[SERVERS_NUM+1];
	

	pthread_t threads_servers[SERVERS_NUM + 1];

	int *s;
	short port_server = FIRST_PORT;
	  
	for(int i = 0; i < SERVERS_NUM + 1; i++) {
		port_server = FIRST_PORT + i;
		
		if(i == 0) {
			port_server = FIRST_PORT;
		}
		
		pthread_mutex_init(&(param[i].mutex), NULL);
		param[i].status = 1;
		param[i].port = port_server;
		//printf("port %d - status %d\n",param[i].port, param[i].status);
	}

	pthread_create(&threads_servers[0], NULL, ThreadServerMain, &param);
	
	for(int i = 1; i < SERVERS_NUM + 1; i++) {
		pthread_create(&threads_servers[i], NULL, ThreadServer, &param[i]);
	}
	
	for(int i = 1; i < SERVERS_NUM; i++) {
		pthread_join(threads_servers[i], (void **) &s);
	}
	
	for(int i = 0; i < SERVERS_NUM; i++) {
		pthread_mutex_destroy(&(param[i].mutex));
	}
	
	return 0;
}
