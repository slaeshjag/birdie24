#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef _WIN32
#include <pthread.h>
#endif

#include "client.h"
#include "network.h"
#include "proto.h"

#ifndef _WIN32
pthread_t client_recv_thread;
#endif

void *client_recv(void *arg) {
	struct proto_packet packet;
	for(;;) {
		network_recv(&packet, sizeof(struct proto_packet));
		printf("got shit\n");
	}
	return NULL;
}

void client_init() {
	#ifdef _WIN32
	#warning Not spawning client_recv
	#else
	pthread_create(&client_recv_thread, NULL, client_recv, NULL);
	#endif
}
