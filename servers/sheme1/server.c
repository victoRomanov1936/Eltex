#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int port = 4444;

int main() {
	int client;
	int pid;
	struct sockaddr_in serv_adr = {0}, client_adr;
	time_t now;
	
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(3333);
	inet_pton(AF_INET, "127.0.0.1", &serv_adr.sin_addr);
	
	int server = socket(AF_INET, SOCK_STREAM, 0);
	
	if(server == -1) {
		perror("server_fd");
		return -1;
	}

	if(bind(server,(struct sockaddr*) &serv_adr, sizeof serv_adr) == -1) {
		perror("bind");
		return -1;
	}

	if(listen(server, 5) == -1) {
		perror("listen");
		return -1;
	}
	
	socklen_t adrlen =  sizeof serv_adr;
		
	for(;;) {
		client = accept(server, (struct sockaddr*) &client_adr, &adrlen);

		if(client == -1) {
			perror("accept");
			return  -1;
		}
		
		pid = fork();

		if(pid == 0) {
			char buf[256];
			time(&now);
			sprintf(buf,"%s", ctime(&now));
			
			struct sockaddr_in serv_adr1 = {0};
			serv_adr1.sin_family = AF_INET;
			serv_adr1.sin_port = htons(port);
			inet_pton(AF_INET, "127.0.0.1", &serv_adr1.sin_addr);

			int server1 = socket(AF_INET, SOCK_STREAM, 0);

			if(server1 == -1) {
				perror("server_fd");
				return -1;
			}

			if(bind(server1,(struct sockaddr*) &serv_adr1, \
			sizeof serv_adr1) == -1) {
				perror("bind");
				return -1;
			}
					
			if(send(client, buf, 256, 0) == -1) {
				perror("send");
				return -1;
			}

			sleep(1);
			close(client);
			close(server1);
					
			return 0;
				
		} else {
			printf("port server: %d\n", port);
			port++;
		}
	}
	
	return 0;
}
