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
	
	unsigned char lenght[sizeof(short) + 1];
	unsigned char sourse_p[sizeof(short) + 1];
	unsigned char destination_p[sizeof(short) + 1];

	char *lng = &lenght_pack;
	char *sp = &source_port;
	char *dp = &destination_port;

	fbytech(sp, sourse_p);
	fbytech(dp, destination_p);
	fbytech(lng, lenght);
	printf("\n");

	for(int i = 0; i< 264; i++) {
		 buf[i] = 0;
	}

	sprintf(buf,"%s%s%s%c%c%s",sourse_p, destination_p, lenght, 1,1,"C:HELLO WORLD\n");
	buf[6]=0;
	buf[7]=0;

	for(int i = 0; i< strlen(buf); i++) {
		 printf("%x ", buf[i]);
	}

	struct sockaddr_in serv;
	
	int client_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	sendto(client_fd, buf, sizeof(buf), 0, (struct sockaddr*)&serv, sizeof(serv));

	printf("send to ok\n");
	
	
	close(client_fd);
	return 0;
}

