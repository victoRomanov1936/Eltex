#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	struct sockaddr_in serv, client;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(55555);
	inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);
	
	int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(server_fd == -1) {
		perror("server_fd");
		return -1;
	}

	if(bind(server_fd,(struct sockaddr*) &serv, sizeof(serv)) == -1) {
		perror("bind");
		return -1;
	}

	socklen_t adrlen =  sizeof(client);

	char buf[256];
	while(1) {
		recvfrom(server_fd, &buf, 256, 0,(struct sockaddr*)&client, &adrlen);
		buf[0] = 'S';
		printf("%s", buf);
	
	//client.sin_port = htons(7777);
	//sendto(server_fd, buf, 256, 0, (struct sockaddr*)&client, adrlen);
	}

	sleep(1);
	close(server_fd);

	return 0;
}

