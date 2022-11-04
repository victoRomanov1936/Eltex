#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	struct sockaddr_in client_adr = {0};
	int client = socket(AF_INET, SOCK_STREAM, 0);
	
	if(client == -1) {
		perror("client_fd");
		return -1;
	}
	
	client_adr.sin_family = AF_INET;
	client_adr.sin_port = htons(33344);
	inet_pton(AF_INET, "127.0.0.1", &client_adr.sin_addr);
	
	if(connect(client, (struct sockaddr*)&client_adr, sizeof client_adr) == -1){
		perror("connect");
		return -1;
	}



	char buf[256] = {"C:HELLO\n"};

	if(send(client, buf, 9, 0) == -1) {
		perror("send");
		return -1;
	}

	if(recv(client, buf, 256, 0) == -1) {
		perror("recv");
		return -1;
	}

	printf("%s", buf);

	close(client);
	return 0;
}
