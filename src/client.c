#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "client.h"
#include "main.h"
#include "network.h"
#include "proto.h"
#include "server.h"

pthread_t client_recv_thread;

void *client_recv(void *arg) {
	struct proto_packet packet;
	struct proto_join_packet *packet_join;
	unsigned long addr;
	
	for(;;) {
		addr = network_recv(&packet, sizeof(struct proto_packet));
		
		switch(*((enum proto_packet_type *) &packet)) {
			case PROTO_TYPE_BROADCAST:
				if(config.game_state == GAME_STATE_LOBBY_HOST && !strcmp(config.server.name, ((struct proto_control_broadcast *) &packet)->game_name)) {
					config.server.addr = addr;
					config.server.connected = true;
				}
				break;
			case PROTO_TYPE_JOIN:
				packet_join = (void *) &packet;
				if(packet_join->player_id == -1) {
					server_handle_packet(&packet, addr);
					break;
				}
				
				strcpy(config.player.player[packet_join->player_id], packet_join->player_name);
				
				break;
			default:
				server_handle_packet(&packet, addr);
				break;
		}
	}
	return NULL;
}

void client_init() {
	int i;
	
	pthread_create(&client_recv_thread, NULL, client_recv, NULL);
	
	for(i = 0; i < FARMER_COUNT; i++) {
		config.player.player[i][0] = 0;
	}
}
