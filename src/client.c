#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "client.h"
#include "main.h"
#include "network.h"
#include "proto.h"
#include "server.h"
#include "lobby.h"
#include "game.h"

#ifdef _WIN32

#include <windows.h>

#define pthread_t HANDLE
#define pthread_create(handle, nc1, func, nc2) (*(handle) = CreateThread(NULL, 0, (func), NULL, 0, NULL))
#define pthread_cancel(thread) (TerminateThread((thread), 0))

#else

#include <pthread.h>

#endif

pthread_t client_recv_thread;

char megabuf[512];

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
	int i;
	struct proto_packet packet;
	struct proto_packet *packet_game;
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
			case PROTO_TYPE_GAMESTATE:
				if(config.game_state == GAME_STATE_LOBBY_JOIN) {
					game_state(GAME_STATE_GAME);
				}
				packet_game = (void *) &packet;
				for(i = 0; i < SHEEP_COUNT; i++) {
					game.sheep.sheep[i].x = packet_game->sheep[i].x;
					game.sheep.sheep[i].y = packet_game->sheep[i].y;
					game.sheep.sheep[i].angle =  ((packet_game->sheep[i].dir)*225) - 900;
				}
				
				for(i = 0; i < FARMER_COUNT; i++) {
					game.farmer.farmer[i].x = packet_game->farmer[i].coord.x;
					game.farmer.farmer[i].y = packet_game->farmer[i].coord.y;
					game.farmer.farmer[i].move = packet_game->farmer[i].coord.moving;
					game.farmer.farmer[i].angle = ((packet_game->farmer[i].coord.dir)*225) - 900;
					game.farmer.farmer[i].yell = packet_game->farmer[i].action_yell;
					game.farmer.farmer[i].stab = packet_game->farmer[i].action_stab;
					config.points[i] = packet_game->sheep_points[i];
				}
				config.timer = packet_game->time_remain;
				if(config.timer <= 0) {
					if(config.points[0] > config.points[1])
						sprintf(megabuf, "Player %s wins!", config.player.player[0]);
					else if(config.points[0] == config.points[1])
						sprintf(megabuf, "The result was tied.");
					else
						sprintf(megabuf, "Player %s wins!", config.player.player[1]);
					
					config.text_aux = megabuf;
					
					game_state(GAME_STATE_MENU);
				}
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
	lobby_client.client = NULL;
	add_lobby_game("[Host server]", 0);
	lobby_client.clients = 0;

	pthread_create(&client_recv_thread, NULL, client_recv, NULL);
	
	for(i = 0; i < FARMER_COUNT; i++) {
		config.player.player[i][0] = 0;
	}
}
