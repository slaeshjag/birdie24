#include "proto.h"
#include "network.h"
#include "server.h"
#include "main.h"
#include <string.h>


void server_init() {
	memset(&server_state, 0, sizeof(server_state));
	server_state.pp.type = PROTO_TYPE_GAMESTATE;
	return;
}


void server_kick(int player) {
	int i;
	struct proto_join_packet jp;

	server_state.plist[player].used = 0;
	jp.type = PROTO_TYPE_JOIN;
	*jp.player_name = 0;
	jp.player_id = -1; 

	for (i = 0; i < FARMER_COUNT; i++)
		network_send(server_state.plist[player].addr, &jp, sizeof(jp));
	return;
}


void server_check_alive() {
	int i;

	for (i = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].used)
			if (server_state.plist[i].timeout++ >= SERVER_FARMER_SLEEPING)
				server_kick(i);
	return;
}


int server_get_players() {
	int i, j;

	for (i = j = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].used)
			j++;
	return j;
}


void server_handle_join(struct proto_join_packet *pj, unsigned long addr) {
	int i;
	struct proto_join_greet jg;

	for (i = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].used)
			if (server_state.plist[i].addr == addr)
				return;
	for (i = 0; i < FARMER_COUNT; i++)
		if (!server_state.plist[i].used)
			break;
	if (i == FARMER_COUNT)
		return;
	pj->player_name[PROTO_PLAYER_NAME - 1] = 0;
	strcpy(server_state.plist[i].player_name, pj->player_name);
	server_state.plist[i].addr = addr;
	server_state.plist[i].timeout = 0;
	pj->addr = addr;

	/* Emit join greet */
	jg.type = PROTO_TYPE_JOIN_GREET;
	jg.player_num = i;
	network_send(addr, &jg, sizeof(jg));

	pj->player_id = i;
	server_state.plist[i].used = 1;
	for (i = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].used)
			network_send(server_state.plist[i].addr, pj, sizeof(*pj));

	return;
}


void server_handle_control(struct proto_control_packet *cp, int player_id) {
	if (player_id < 0)
		return;
	server_state.plist[player_id].up = cp->up;
	server_state.plist[player_id].down = cp->down;
	server_state.plist[player_id].left = cp->left;
	server_state.plist[player_id].right = cp->right;
	server_state.pp.farmer[player_id].action_yell = cp->yell;
	server_state.pp.farmer[player_id].action_stab = cp->stab;
	return;
}

int server_get_player(unsigned long addr) {
	int i;

	for (i = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].addr == addr)
			return i;
	return -1;
}


void server_handle_packet(void *packet, unsigned long addr) {
	int p;

	switch (*((enum proto_packet_type *) packet)) {
		case PROTO_TYPE_BROADCAST:
		case PROTO_TYPE_LOBBY_STAT:
		case PROTO_TYPE_GAMESTATE:
		case PROTO_TYPE_JOIN_GREET:
		default:
			fprintf(stderr, "Herpaderp wrong packet type %i\n", *((enum proto_packet_type *) packet));
			break;
		case PROTO_TYPE_CONTROL:
			server_handle_control(packet, server_get_player(addr));
			break;
		case PROTO_TYPE_KEEPALIVE:
			if ((p = server_get_player(addr)) < 0) {
				fprintf(stderr, "Keepalive from irrelevant player with addr 0x%X\n", (unsigned int) addr);
				return;
			}

			server_state.plist[p].timeout = 0;
			break;
		case PROTO_TYPE_JOIN:
			server_handle_join(packet, addr);
			break;
	}

	return;
}


void server_start(const char *game_name) {
	server_init();
	server_state.enabled = 1;
	strcpy(server_state.name, game_name);

	return;
}


void server_loop() {
	static int cnt;
	struct proto_control_broadcast cb;
	int i;

	if (!server_state.enabled)
		return;
	server_check_alive();

	switch (config.game_state) {
		case GAME_STATE_GAME:
			farmer_move();
			break;
		case GAME_STATE_LOBBY_HOST:
			if (!(cnt & 0xF)) {
				cb.type = PROTO_TYPE_BROADCAST;
				strcpy(cb.game_name, server_state.name);
				strcpy(cb.map_name, "arne.ldmz");
				cb.slots = FARMER_COUNT;
				cb.slots_left = FARMER_COUNT - server_get_players();
				network_broadcast(&cb, sizeof(cb));
			}
			break;
		default:
			break;
	}

	for (i = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].used)
			switch (config.game_state) {
				case GAME_STATE_GAME:
					network_send(server_state.plist[i].addr, &server_state.pp, sizeof(server_state.pp));
					break;
				case GAME_STATE_LOBBY_HOST:
					break;
				default:
					fprintf(stderr, "Shitter's full\n");
			}
	
	cnt++;
	

	return;
}
