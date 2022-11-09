#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 4444
#define ADRESS "127.0.0.1"

int main() {
	struct sockaddr_in client_adr = {0};
	int client = socket(AF_INET, SOCK_STREAM, 0);
	
	if(client == -1) {
		perror("client_fd");
		return -1;
	}
	
	client_adr.sin_family = AF_INET;
	client_adr.sin_port = htons(PORT);
	inet_pton(AF_INET, ADRESS, &client_adr.sin_addr);
	
	if(connect(client, (struct sockaddr*)&client_adr, sizeof client_adr) == -1){
		perror("connect");
		return -1;
	}

	time_t now;
	
	if(recv(client, &now, sizeof(time_t), 0) < 0) {
		perror("recv");
		return -1;
	}

	printf("%s\n", ctime(&now));
	close(client);
	return 0;
}
