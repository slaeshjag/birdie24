#include "proto.h"
#include <stdio.h>
#include <stdlib.h>

void proto_init() {
	fprintf(stderr, "proto packet has size %lu\n", sizeof(struct proto_packet));

	return;
}


/* TODO: Add arg for IP, port, etc */
void proto_send(struct proto_packet *pp) {
	/* TODO: Do UDP stuff */
	/* TODO: Compress packet? */

	return;
}



/* TODO: Add arg to get IP */
void proto_recv(struct proto_packet *pp) {
	/* TODO: Do UDP stuff */
	/* TODO: Decompress stuff? */

	return;
}
