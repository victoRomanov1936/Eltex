#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/if.h>
#endif

struct UdpHeader {
	unsigned char source_port[2];
	unsigned char destination_port[2];
	unsigned char lenght[2];
	unsigned char check_sum[2];
}udp_h;

struct IpHeader {
	unsigned char version_ihl;
	unsigned char ds;
	unsigned char lenght[2];
	unsigned char identification[2];
	unsigned char flags;
	unsigned char offset;
	unsigned char time_to_live;
	unsigned char transport_protocol;
	unsigned char check_sum[2];
	unsigned char source[4];
	unsigned char destination[4];
}ip_h;


void conversion_to_short(unsigned char arr[], short value) {
	char *point_arr = &value;
	for(int i = 0; i< sizeof(short); i++) {
		arr[i] = point_arr[i];
	}
}


int main() {

	unsigned char buf[298];
	unsigned char buf_msg[] = "C:HELLO WORLD!\n";
	

	for (int i = 0; i < sizeof(buf); i++) buf[i] = 0;

	//INIT ETHERNET VALUE
	unsigned char source_mac[6] = {0, 0x1b, 0xb9, 0x64, 0x34, 0x12};
	unsigned char destination_mac [6]= {0x30, 0x65, 0xec, 0x93, 0xa0, 0x0d};
	unsigned char type[2] = {8, 0};

	
	//INIT IP VALUE
	ip_h.version_ihl= 0x45;
	ip_h.ds = 0;
	conversion_to_short(ip_h.lenght, htons(284));
	conversion_to_short(ip_h.identification, htons(4444));
	ip_h.flags = 0;
	ip_h.offset = 0;
	ip_h.time_to_live = 255;
	ip_h.transport_protocol = 17;
	conversion_to_short(ip_h.check_sum , htons(0x279c));
	inet_pton(AF_INET, "192.168.0.44", &ip_h.source);
	inet_pton(AF_INET, "192.168.0.92", &ip_h.destination);

	//INIT UDP VALUE
	conversion_to_short(udp_h.source_port, htons(7777));
	conversion_to_short(udp_h.destination_port, htons(55555));
	conversion_to_short(udp_h.lenght, htons(264));
	conversion_to_short(udp_h.check_sum, htons(0));
	
	int point = 0;
	//ETHERNET
	memcpy(buf ,destination_mac, 6);
	memcpy(buf + (point+=6) ,source_mac, 6);
	memcpy(buf + (point+=6) ,type, 2);
	

	//IP
	buf[point+=2] = ip_h.version_ihl;
	buf[point+=1] = ip_h.ds;
	memcpy(buf + (point+=1) ,ip_h.lenght, 2);
	memcpy(buf + (point+=2) ,ip_h.identification, 2);
	buf[point+=2] = ip_h.flags;
	buf[point+=1] = ip_h.offset;
	buf[point+=1] = ip_h.time_to_live;
	buf[point+=1] = ip_h.transport_protocol;
	memcpy(buf + (point+=1) ,ip_h.check_sum, 2);
	memcpy(buf + (point+=2) ,ip_h.source, 4);
	memcpy(buf + (point+=4) ,ip_h.destination, 4);
	
	//UDP
	memcpy(buf + (point+=4) , udp_h.source_port, 2);
	memcpy(buf + (point+=2), udp_h.destination_port, 2);
	memcpy(buf + (point+=2), udp_h.lenght, 2);
	memcpy(buf + (point+=2), udp_h.check_sum, 2);

	//MSG
	memcpy(buf + (point+=2)	, buf_msg, sizeof(buf_msg));

	for(int i = 0; i<sizeof(buf); i++) {
		printf("%x ", buf[i]);
	}

	printf("\n");

	struct sockaddr_ll serv = {0};
	
	serv.sll_family = AF_PACKET;
	serv.sll_ifindex = if_nametoindex("enp0s4");
	//unsigned short int sll_hatype;
	//unsigned char sll_pkttype;
	serv.sll_halen = 6;
	//unsigned char sll_addr[8];
	
	int client_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	socklen_t adrlen = sizeof(serv);
	
	sendto(client_fd, buf, sizeof(buf), 0, (struct sockaddr*)&serv, adrlen);
	for (int i = 0; i < sizeof(buf); i++) buf[i] = 0;
	perror("sendto");	
	
	while(1) {
		recvfrom(client_fd, &buf, sizeof(buf), 0,(struct sockaddr*)&serv, &adrlen);
		
			if(buf[23] == 17 && buf[36]== 0x22 && buf[37] == 0xb8) {
				for(int i = 42; i< 284; i++) {
					printf("%c", buf[i]);
				}
				
				close(client_fd);
				return 1;
			}
	}
	
	return 0;
}

