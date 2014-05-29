#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "client.h"
#include "network.h"
#include "proto.h"

pthread_t client_recv_thread;

void *client_recv(void *arg) {
	struct proto_packet packet;
	for(;;) {
		network_recv(&packet, sizeof(struct proto_packet));
		printf("got shit\n");
	}
	return NULL;
}

void client_init() {
	pthread_create(&client_recv_thread, NULL, client_recv, NULL);
}
