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

	for (i = 0; i < FARMER_COUNT; i++)
		server_state.pp.leader_sheep[i] = SHEEP_COUNT / FARMER_COUNT * i;

	fprintf(stderr, "baaaaaaaaaaaaah!\n");
	return;
}
		


void sheep_loop() {
	int i, j, dx, dy, f_dx, f_dy, m_dx, m_dy;

	/* Counting sheep... Zzz... */
	for (i = 0; i < SHEEP_COUNT; i++) {
		f_dx = f_dy = 0;
		for (j = 0; j < FARMER_COUNT && !server_state.sheep[i].panic && 0; j++) {
			/* Farmer anti-gravity */
			dx = ABS(server_state.sheep[i].x - server_state.plist[j].x);
			dy = ABS(server_state.sheep[i].y - server_state.plist[j].y);
			dx /= 1000;
			dy /= 1000;
			if (dx*dx + dy*dy > SHEEP_AVOID_RADIUS * SHEEP_AVOID_RADIUS)
				goto no_antigravity;

			dx = SHEEP_AVOID_RADIUS - dx;
			dy = SHEEP_AVOID_RADIUS - dy;
			if (server_state.sheep[i].x - server_state.plist[j].x < 0)
				dx *= -1;
			if (server_state.sheep[i].y - server_state.plist[j].y < 0)
				dy *= -1;
			f_dx += dx;
			f_dy += dy;
			
			no_antigravity:
			/* leader sheep anti-anti-gravity */
			dx = ABS(server_state.sheep[i].x - server_state.sheep[server_state.pp.leader_sheep[j]].x);
			dy = ABS(server_state.sheep[i].y - server_state.sheep[server_state.pp.leader_sheep[j]].y);
			dx /= 1000;
			dy /= 1000;

			if (dx*dx + dy*dy > (SHEEP_AVOID_RADIUS*3) * (SHEEP_AVOID_RADIUS*3))
				continue;

			dx = SHEEP_AVOID_RADIUS*3 - dx;
			dy = SHEEP_AVOID_RADIUS*3 - dy;
			if (server_state.sheep[i].x - server_state.sheep[server_state.pp.leader_sheep[j]].x > 0)
				dx *= -1;
			if (server_state.sheep[i].y - server_state.sheep[server_state.pp.leader_sheep[j]].y > 0)
				dy *= -1;
			f_dx += dx;
			f_dy += dy;
		}

		if (server_state.sheep[i].panic) {
			f_dx = server_state.sheep[i].pg_x;
			f_dy = server_state.sheep[i].pg_y;
			server_state.sheep[i].panic = 0;
		} else {
			server_state.sheep[i].pg_x = 0;
			server_state.sheep[i].pg_y = 0;
		}

		m_dx = f_dx;
		m_dy = f_dy;
		server_state.pp.sheep[i].angle = 

		server_state.sheep[i].x += m_dx;
		server_state.sheep[i].y += m_dy;

	}
	
	/* Update sheep location packet */
	for (i = 0; i < SHEEP_COUNT; i++) {
		server_state.pp.sheep[i].x = server_state.sheep[i].x / 1000;
		server_state.pp.sheep[i].y = server_state.sheep[i].y / 1000;
		server_state.pp.sheep[i].dir = 0;
		server_state.pp.sheep[i].moving = 0;
	}
	return;
}
