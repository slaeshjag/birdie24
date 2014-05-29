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
	}, recvaddr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	char buf[16];
	char frombuf[32];
	
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
		socklen_t len = sizeof(recvaddr);
		recvfrom(sock, buf, 16, 0, (struct sockaddr *) &recvaddr, &len);
		inet_ntop(AF_INET, &recvaddr, frombuf, 32);
		printf("got '%s' from %s\n", buf, frombuf);
	}
	
	close(sock);
	return 0;
}
