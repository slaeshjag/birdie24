#include "proto.h"
#include "network.h"
#include "server.h"
#include <string.h>

void server_fill_packet() {
	/* TODO: Copy over the internal server state to here */

	return;
}


void server_handle_join(struct proto_join_packet *pj, int len, unsigned long addr) {
	int i;

	if (len < sizeof(*pj)) {
		fprintf(stderr, "Packet too short in server_handle_join\n");
		return;
	}

	for (i = 0; i < FARMER_COUNT; i++)
		if (!server_state.plist[i].used)
			break;
	if (i == FARMER_COUNT)
		return;
	pj->player_name[PROTO_PLAYER_NAME - 1] = 0;
	strcpy(server_state.plist[i].player_name, pj->player_name);
	server_state.plist[i].addr = addr;

	/* TODO: Emit join greet */

	return;
}


int server_get_player(unsigned long addr) {
	int i;

	for (i = 0; i < FARMER_COUNT; i++)
		if (server_state.plist[i].addr == addr)
			return i;
	return -1;
}


void server_handle_packet(void *packet, int len, unsigned long addr) {
	switch (*((enum proto_packet_type *) packet)) {
		case PROTO_TYPE_BROADCAST:
		case PROTO_TYPE_LOBBY_STAT:
		case PROTO_TYPE_GAMESTATE:
		case PROTO_TYPE_JOIN_GREET:
		default:
			fprintf(stderr, "Herpaderp wrong packet type %i\n", *((enum proto_packet_type *) packet));
			break;
		case PROTO_TYPE_CONTROL:
			fprintf(stderr, "STUB: Handle control packet\n");
			break;
		case PROTO_TYPE_KEEPALIVE:
			fprintf(stderr, "STUB: Handle keep-alive packet\n");
			break;
		case PROTO_TYPE_JOIN:
			server_handle_join(packet, len, addr);
			break;
	}

	return;
}


void server_loop() {
	struct proto_packet pp;
	/* TODO: state machine? */

	/* Do this per player */
	proto_send(&pp);

	return;
}
