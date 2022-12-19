#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(55555);
	inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);
	socklen_t size = sizeof(serv);
	
	int client_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	connect(client_fd,(struct sockaddr*) &serv, size);
	char buf[256] = {"C:HELLO WORLD\n"};
	
	sendto(client_fd, buf, 256, 0, (struct sockaddr*)&serv, size);
	
	printf("%s", buf);
	close(client_fd);
	return 0;
}
