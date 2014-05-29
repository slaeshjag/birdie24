#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
	int sock;
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = 1337,
	}, sendaddr = {
		.sin_family = AF_INET,
		.sin_port = 1337,
	};
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "255.255.255.255", &sendaddr.sin_addr);
	char buf[16] = "arne";
	
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("nein.\n");
		return 1;
	}
	
	if(bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		printf("njet.\n");
		close(sock);
		return -1;
	}
	
	for(;;) {
		sendto(sock, buf, 16, 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));
		printf("sent shit\n");
		sleep(1);
	}
	
	close(sock);
	return 0;
}
