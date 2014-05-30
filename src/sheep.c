#include "farmer.h"
#include "server.h"
#include "sheep.h"
#include "main.h"
#include <limits.h>
#include <darnit/darnit.h>
#include <math.h>

#define	ABS(x)		((x) < 0 ? -(x) : (x))

#define	MAP_COLL(dx, dy)									\
	tx = (server_state.sheep[sheep].x / 1000 + (dx)) / 16;					\
	ty = (server_state.sheep[sheep].y / 1000 + (dy)) / 16;					\
	if (config.map->layer[1].tilemap->data[config.map->layer[1].tilemap->w * ty + tx])	\
		return 1;									

#define	MAP_COLL2(x, y, dx, dy)									\
	tx = (x + (dx)) / 16;								\
	ty = (y + (dy)) / 16;								\
	if (config.map->layer[1].tilemap->data[config.map->layer[1].tilemap->w * ty + tx])	\
		return 1;									

static DARNIT_BBOX *bbox;


void sheep_prepare() {
	bbox = d_bbox_new(SHEEP_COUNT);
	d_bbox_set_indexkey(bbox);
	return;
}


void sheep_init() {
	d_bbox_clear(bbox);
	return;
}


void sheep_spawn() {
	int i, h, c;

	sheep_init();
	fprintf(stderr, "Spawning a herd of sheep... ");

	c = 0;
	for (i = 19; c < SHEEP_COUNT; i++) {
		for (h = 19; h < 34 && c < SHEEP_COUNT; h++) {
			if (config.map->layer[1].tilemap->data[i * config.map->layer[1].tilemap->w + h])
				continue;
			server_state.sheep[c].x = h * 16 * 1000;
			server_state.sheep[c].y = i * 16 * 1000;
			d_bbox_add(bbox, server_state.sheep[i].x / 1000, server_state.sheep[i].y / 1000, 16, 16);
			c++;
		}
	}
		

	for (i = 0; i < FARMER_COUNT; i++)
		server_state.pp.leader_sheep[i] = SHEEP_COUNT / FARMER_COUNT * i;
	
	sheep_loop();
	
	fprintf(stderr, "baaaaaaaaaaaaah!\n");
	return;
}

int sheep_test_collide(int x, int y, int dx, int dy, int t) {
	int c;
	int tx, ty;
	c =  d_bbox_test(bbox, x + dx, y + dy, 16, 16, server_state.sheep_collide, SHEEP_COUNT);
	if (c > t) {
		return 1;
	}

	if (dx < 0) {
		MAP_COLL2(x, y, -1, 0);
		MAP_COLL2(x, y, -1, 15);
	} else if (dx > 0) {
		MAP_COLL2(x, y, 16, 0);
		MAP_COLL2(x, y, 16, 15);
	}

	if (dy < 0) {
		MAP_COLL2(x, y, 0, -1);
		MAP_COLL2(x, y, 15, -1);
	} else if (dy > 0) {
		MAP_COLL2(x, y, 0, 16);
		MAP_COLL2(x, y, 15, 16);
	}
	
	if (x + dx >= config.platform.screen_w - 16)
		return 1;
	if (y + dy >= config.platform.screen_h - 16)
		return 1;
	if (x + dx < 1)
		return 1;
	if (y + dy < 1)
		return 1;
	return 0;
}

void sheep_move() {
	int i;
	int move = 1;

	while (move) {
		move = 0;
		for (i = 0; i < SHEEP_COUNT; i++) {
			if (ABS(server_state.sheep[i].dx) >= 1000) {
				if (!sheep_test_collide(server_state.sheep[i].x / 1000, server_state.sheep[i].y / 1000, (server_state.sheep[i].dx < 0) ? -1 : 1, 0, 1))
					server_state.sheep[i].x += (server_state.sheep[i].dx < 0) ? -1000 : 1000;
				server_state.sheep[i].dx += (server_state.sheep[i].dx < 0) ? 1000 : -1000;
				move = 1;
					
			}
			
			if (ABS(server_state.sheep[i].dy) >= 1000) {
				if (!sheep_test_collide(server_state.sheep[i].x / 1000, server_state.sheep[i].y / 1000, 0, (server_state.sheep[i].dy < 0) ? -1 : 1, 1))
					server_state.sheep[i].y += (server_state.sheep[i].dy < 0) ? -1000 : 1000;
				server_state.sheep[i].dy += (server_state.sheep[i].dy < 0) ? 1000 : -1000;
				move = 1;
			}

			d_bbox_move(bbox, i, server_state.sheep[i].x / 1000, server_state.sheep[i].y / 1000);
		}
	}

	return;
}


void sheep_loop() {
	int i, j, dx, dy, f_dx, f_dy, m_dx, m_dy, d;
	float x, y;

	/* Counting sheep... Zzz... */
	for (i = 0; i < SHEEP_COUNT; i++) {
		f_dx = f_dy = 0;
		for (j = 0; j < FARMER_COUNT && !server_state.sheep[i].panic; j++) {
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
			dx *= 6;
			dy *= 6;
			f_dx += dx;
			f_dy += dy;
	
			no_antigravity:
			if (server_state.pp.leader_sheep[j] == i)
				continue;
			/* leader sheep anti-anti-gravity */
			dx = ABS(server_state.sheep[i].x - server_state.sheep[server_state.pp.leader_sheep[j]].x);
			dy = ABS(server_state.sheep[i].y - server_state.sheep[server_state.pp.leader_sheep[j]].y);
			dx /= 1000;
			dy /= 1000;

			if (dx*dx + dy*dy > (SHEEP_AVOID_RADIUS) * (SHEEP_AVOID_RADIUS) * 3)
				continue;

			dx = SHEEP_AVOID_RADIUS - dx;
			dy = SHEEP_AVOID_RADIUS - dy;
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
		if (!m_dx) {
			if (m_dy >= 0)
				d = 0;
			else
				d = 8;
		} else {
			x = m_dx;
			y = m_dy;
			d = atan(y/x) * 1800. / M_PI;
			if (x > 0 && d < 0)
				d += 3600;
			if (x < 0)
				d += 1800;
			d += 112;
			d /= 225;
			d %= 16;
			//fprintf(stderr, "%i = dir %i\n", deg, d);
		}
		server_state.pp.sheep[i].dir = d;

		server_state.sheep[i].dx += m_dx;
		server_state.sheep[i].dy += m_dy;
	}
	
	sheep_move();

	/* Update sheep location packet */
	for (i = 0; i < SHEEP_COUNT; i++) {
		server_state.pp.sheep[i].x = server_state.sheep[i].x / 1000;
		server_state.pp.sheep[i].y = server_state.sheep[i].y / 1000;
		server_state.pp.sheep[i].moving = 0;
	}
	return;
}


void sheep_panic(int x, int y) {
	int c, i;
	c =  d_bbox_test(bbox, x - SHEEP_PANIC_RADIUS / 2, y - SHEEP_PANIC_RADIUS / 2, SHEEP_PANIC_RADIUS, SHEEP_PANIC_RADIUS, server_state.sheep_collide, SHEEP_COUNT);

	for (i = 0; i < c; i++) {
		server_state.sheep[server_state.sheep_collide[i]].panic = 1;
		if (!server_state.sheep[server_state.sheep_collide[i]].pg_x)
			server_state.sheep[server_state.sheep_collide[i]].pg_x = rand() % SHEEP_PANIC_RANGE * ((rand() & 1) ? -1 : 1) + 2000 * ((rand() & 1) ? -1 : 1);
		if (!server_state.sheep[server_state.sheep_collide[i]].pg_y)
			server_state.sheep[server_state.sheep_collide[i]].pg_y = rand() % SHEEP_PANIC_RANGE * ((rand() & 1) ? -1 : 1) + 2000 * ((rand() & 1) ? -1 : 1);
	}

	return;
}


void sheep_counter() {
	server_state.pp.sheep_points[0] = d_bbox_test(bbox, 0*16, 16*16, 7*16, 7*16, server_state.sheep_collide, SHEEP_COUNT);
	server_state.pp.sheep_points[1] = d_bbox_test(bbox, 43*16, 16*16, 7*16, 7*16, server_state.sheep_collide, SHEEP_COUNT);

	return;
}
