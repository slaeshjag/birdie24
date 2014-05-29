#include "proto.h"


void server_fill_packet() {
	/* TODO: Copy over the internal server state to here */

	return;
}


void server_loop() {
	struct proto_packet pp;
	/* TODO: state machine? */

	/* Do this per player */
	proto_send(&pp);

	return;
}
