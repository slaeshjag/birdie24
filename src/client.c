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
#include "lobby.h"

pthread_t client_recv_thread;

static void add_lobby_game(char *name, unsigned long addr) {
	struct List **list;
	for(list = &lobby_client.client; *list; list = &((*list)->next)) {
		if(!strcmp(name, ((struct LobbyHost *) ((*list)->value))->name)) {
			goto replace_addr;
		}
	}
	*list = malloc(sizeof(struct List));
	(*list)->value = malloc(sizeof(struct LobbyHost));
	(*list)->next = NULL;
	((struct LobbyHost *) ((*list)->value))->name = name;
	lobby_client.clients++;
	replace_addr:
	((struct LobbyHost *) ((*list)->value))->addr = addr;
}

void *client_recv(void *arg) {
	struct proto_packet packet;
	struct proto_join_packet *packet_join;
	struct proto_control_broadcast *packet_broadcast;
	unsigned long addr;
	
	for(;;) {
		addr = network_recv(&packet, sizeof(struct proto_packet));
		
		switch(*((enum proto_packet_type *) &packet)) {
			case PROTO_TYPE_BROADCAST:
				packet_broadcast = (void *) &packet;
				switch(config.game_state) {
					case GAME_STATE_LOBBY_HOST:
						if(!strcmp(config.server.name, packet_broadcast->game_name)) {
							config.server.addr = addr;
							config.server.connected = true;
						}
						break;
					case GAME_STATE_LOBBY:
						add_lobby_game(packet_broadcast->game_name, addr);
						break;
					default:
						break;
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
	pthread_cancel(client_recv_thread);
	/*Freeing memory is for pussies*/
	add_lobby_game("[Host server]", 0);
	lobby_client.clients = 0;

	pthread_create(&client_recv_thread, NULL, client_recv, NULL);
	
	for(i = 0; i < FARMER_COUNT; i++) {
		config.player.player[i][0] = 0;
	}
}
