#include "farmer.h"
#include "server.h"
#include "sheep.h"
#include "main.h"
#include <limits.h>

#define	ABS(x)		((x) < 0 ? (x) : -(x))

struct sheep_gravity {
	int			dx;
	int			dy;
};


void sheep_spawn() {
	int i, h;

	fprintf(stderr, "Spawning a herd of sheep... ");

	/* Räkna fåren! */
	h = SHEEP_COUNT >> 1;
	for (i = 0; i < h; i++) {
		server_state.sheep[i].x = rand() % 200;
		server_state.sheep[i].y = rand() % config.platform.screen_h - 16;
	}

	/* Side B */
	for (; i < SHEEP_COUNT; i++) {
		server_state.sheep[i].x = rand() % 200 - 200 + config.platform.screen_w;
		server_state.sheep[i].y = rand() % config.platform.screen_h - 16;
	}

	fprintf(stderr, "done!\n");
	return;
}
		


void sheep_loop() {
	int i, j;
	struct sheep_gravity g[FARMER_COUNT];

	/* Counting sheep... Zzz... */
	for (i = 0; i < SHEEP_COUNT; i++) {
		for (j = 0; j < FARMER_COUNT; i++) {
//			g[j] = (
		}
		
		/* When we know what farmer to run from */
		/* TODO: Make sheep follow other sheep */


		for (i = 0; i < SHEEP_COUNT; i++) {
			server_state.pp.sheep[i].x = server_state.sheep[i].x / 1000;
			server_state.pp.sheep[i].y = server_state.sheep[i].y / 1000;
			server_state.pp.sheep[i].dir = 0;
			server_state.pp.sheep[i].moving = 0;
		}
	}
	return;
}
