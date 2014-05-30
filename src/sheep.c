#include "farmer.h"
#include "server.h"
#include "sheep.h"
#include "main.h"
#include <limits.h>

#define	ABS(x)		((x) < 0 ? (x) : -(x))


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
	int i, j, dx, dy, f_dx, f_dy, m_dx, m_dy;

	/* Counting sheep... Zzz... */
	for (i = 0; i < SHEEP_COUNT; i++) {
		f_dx = f_dy = 0;
		for (j = 0; j < FARMER_COUNT; j++) {
			dx = ABS(server_state.sheep[i].x - server_state.plist[j].x);
			dy = ABS(server_state.sheep[i].y - server_state.plist[j].y);
			dx /= 1000;
			dy /= 1000;
			if (dx*dx + dy*dy > SHEEP_AVOID_RADIUS)
				continue;

			dx = SHEEP_AVOID_RADIUS - dx;
			dy = SHEEP_AVOID_RADIUS - dy;
			if (server_state.sheep[i].x - server_state.plist[j].x < 0)
				dx *= -1;
			if (server_state.sheep[i].y - server_state.plist[j].y < 0)
				dy *= -1;
			f_dx += dx;
			f_dy += dy;
		}
	

		m_dx = f_dx;
		m_dy = f_dy;

		/* TODO: Add other gravity points, like leader sheeps */
		/* TODO: Make sheep follow other sheep */

		server_state.sheep[i].x += m_dx;
		server_state.sheep[i].y += m_dy;

		/* Update sheep location packet */
	}
	
	for (i = 0; i < SHEEP_COUNT; i++) {
		server_state.pp.sheep[i].x = server_state.sheep[i].x / 1000;
		server_state.pp.sheep[i].y = server_state.sheep[i].y / 1000;
		server_state.pp.sheep[i].dir = 0;
		server_state.pp.sheep[i].moving = 0;
	}
	return;
}
