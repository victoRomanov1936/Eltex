#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct UdpHeader {
	short source_port;
	short destination_port;
	short lenght;
	short check_sum;
};

struct IpHeader {
	char version_ihl;
	char ds;
	short lenght;
	short identification;
	char flags;
	char offset;
	char time_to_live;
	char transport_protocol;
	short check_sum;
	int source;
	int destination;
};


void conversion_to_short(unsigned char arr[], short *udp_field) {
	char *point_arr = udp_field;
	for(int i = 0; i< sizeof(short); i++) {
		arr[i] = point_arr[i];
	}
	arr[sizeof(short)] = '\0';
}

void conversion_to_int(unsigned char arr[], int *udp_field) {
	char *point_arr = udp_field;
	for(int i = 0; i< sizeof(int); i++) {
		arr[i] = point_arr[i];
	}
	arr[sizeof(int)] = '\0';
}


int main() {
	unsigned char buf_ip[284];
	unsigned char buf_udp[284];
	
	int val = 1;
	
	for(int i = 0; i< 284;i++) {
		buf_udp[i] = 0;
		buf_ip[i] = 0;
	}
	
	struct IpHeader ip_h = {
		.version_ihl = 0x45,
		.ds = 0,
		.lenght = htons(284),
		.identification = htons(4444),
		.flags = 0,
		.offset = 0,
		.time_to_live = 255,
		.transport_protocol = 17,
		.check_sum = 0,
	};
	
	inet_pton(AF_INET, "127.0.0.1", &ip_h.source);
	inet_pton(AF_INET, "127.0.0.1", &ip_h.destination);

	
	struct UdpHeader udp_h = {
		.source_port = htons(7777),
		.destination_port = htons(55555),
		.lenght = htons(264),
	};

	unsigned char sourse_p[sizeof(short) + 1];
	unsigned char destination_p[sizeof(short) + 1];
	unsigned char lenght_p[sizeof(short) + 1];
	unsigned char lenght_ip[sizeof(short) + 1];
	unsigned char identification_ip[sizeof(short) + 1];
	unsigned char source_ip[sizeof(int) + 1];
	unsigned char destination_ip[sizeof(int) + 1];
	
	conversion_to_short(sourse_p, &udp_h.source_port);
	conversion_to_short(destination_p, &udp_h.destination_port);
	conversion_to_short(lenght_p, &udp_h.lenght);
	conversion_to_short(lenght_ip, &ip_h.lenght);
	conversion_to_short(identification_ip, &ip_h.identification);
	conversion_to_int(source_ip, &ip_h.source);
	conversion_to_int(destination_ip, &ip_h.destination);
	

	sprintf(buf_ip,"%c%c%s%s%c%c%c%c",ip_h.version_ihl,ip_h.ds,lenght_ip,\
		identification_ip,ip_h.flags,ip_h.offset,ip_h.time_to_live, ip_h.transport_protocol);
	memcpy(buf_ip + 12 ,source_ip,sizeof(source_ip));
	memcpy(buf_ip + sizeof(int) + 12,destination_ip,sizeof(destination_ip));
	
	
	sprintf(buf_udp,"%s%s%s%c%c%s",sourse_p, destination_p, lenght_p, 1,1,"C:HELLO WORLD\n");
	buf_udp[6]=0;
	buf_udp[7]=0;


	memcpy(buf_ip + 20 ,buf_udp, 264);
	
	for(int i = 0; i< 284; i++) {
		 printf("%x ", buf_ip[i]);
	}
	printf("\n");
	
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	
	int client_fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	setsockopt(client_fd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val));
	
	socklen_t adrlen = sizeof(serv);
	
	sendto(client_fd, buf_ip, sizeof(buf_ip), 0, (struct sockaddr*)&serv, adrlen);	
	
	while(buf_udp[22]!= 0x22 && buf_udp[23] != 0xb8) {
		recvfrom(client_fd, &buf_udp, sizeof(buf_udp), 0,(struct sockaddr*)&serv, &adrlen);
	}
	
	for(int i = 28; i< 264; i++) {
		 printf("%c", buf_udp[i]);
	}

	close(client_fd);
	
	return 0;
}

