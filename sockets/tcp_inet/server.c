#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	struct sockaddr_in serv_adr = {0}, client_adr;
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(33344);
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
	int client = accept(server, (struct sockaddr*) &client_adr, &adrlen);

	if(client == -1) {
		perror("accept");
		return -1;
	}

	char buf[256];
	
	if(recv(client, buf, 256, 0) == -1) {
		perror("recv");
		return -1;
	}
	
	printf("%s", buf);
	buf[0] = 'S';

	if(send(client, buf, 256, 0) == -1) {
		perror("send");
		return -1;
	}

	sleep(1);
	close(client);
	close(server);
	
	return 0;
}
