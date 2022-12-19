#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fbytech(char *c, unsigned char arr[]) {
	for(int i = 0; i< sizeof(short); i++) {
		
		arr[i] = c[i];
		printf("%x ",arr[i]);
	}
	arr[sizeof(short)] = '\0';
}

int main() {
	unsigned char buf[264];	
	short source_port = htons(7777);
	short destination_port = htons(55555);
	short lenght_pack = htons(264);
	short sum = htons(65307);
	
	unsigned char sum_p[sizeof(short) + 1];
	unsigned char lenght[sizeof(short) + 1];
	unsigned char sourse_p[sizeof(short) + 1];
	unsigned char destination_p[sizeof(short) + 1];

	char *lng = &lenght_pack;
	char *sm = &sum;
	char *sp = &source_port;
	char *dp = &destination_port;

	fbytech(sp, sourse_p);
	fbytech(dp, destination_p);
	fbytech(lng, lenght);
	fbytech(sm, sum_p);

	sprintf(buf,"%s%s%s%s%s",sourse_p, destination_p, lenght, sum_p, "C:HELLO WORLD\n");
	
	for(int i = strlen(buf); i< 264; i++) {
		 buf[i] = 0;
	}

	struct sockaddr_in serv;
	
	int client_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	sendto(client_fd, buf, sizeof(buf), 0, (struct sockaddr*)&serv, sizeof(serv));

	printf("send to ok\n");
	
	
	close(client_fd);
	return 0;
}

