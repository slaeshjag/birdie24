#include "farmer.h"
#include "server.h"
#include "main.h"
#include <darnit/darnit.h>

static int conv_array[3][3] = {
	{ 5, 6, 7 }, { 4, -1, 0 }, { 3, 2, 1 }
};

#define	ABS(x)		((x) < 0 ? -(x) : (x))

static DARNIT_BBOX *bbox;

void farmer_prepare() {
	bbox = d_bbox_new(FARMER_COUNT);
	d_bbox_set_keyindex(bbox);
}


void farmer_spawn() {
	int i;

	d_bbox_clear(bbox);

	for (i = 0; i < FARMER_COUNT; i++) {
		server_state.plist[i].x = config.platform.screen_w / 2 * 1000;
		server_state.plist[i].y = config.platform.screen_h / FARMER_COUNT * i * 1000 + 1000;
		server_state.plist[i].dx = server_state.plist[i].dy = 0;
		d_bbox_add(bbox, server_state.plist[i].x / 1000, server_state.plist[i].y / 1000, 16, 16);
	}

	farmer_move();
	return;
}


int farmer_get_farmer_facing(int farmer) {
	int x, y, i;
	x = y = 0;
	switch (server_state.pp.farmer[farmer].coord.dir >> 1) {
		case 0:
			x = server_state.pp.farmer[farmer].coord.x + 16;
			y = server_state.pp.farmer[farmer].coord.y;
			break;
		case 1:
			x = server_state.pp.farmer[farmer].coord.x + 16;
			y = server_state.pp.farmer[farmer].coord.y + 16;
			break;
		case 2:
			x = server_state.pp.farmer[farmer].coord.x;
			y = server_state.pp.farmer[farmer].coord.y + 16;
			break;
		case 3:
			x = server_state.pp.farmer[farmer].coord.x - 16;
			y = server_state.pp.farmer[farmer].coord.y + 16;
			break;
		case 4:
			x = server_state.pp.farmer[farmer].coord.x - 16;
			y = server_state.pp.farmer[farmer].coord.y;
			break;
		case 5:
			x = server_state.pp.farmer[farmer].coord.x - 16;
			y = server_state.pp.farmer[farmer].coord.y - 16;
			break;
		case 6:
			x = server_state.pp.farmer[farmer].coord.x;
			y = server_state.pp.farmer[farmer].coord.y - 16;
			break;
		case 7:
			x = server_state.pp.farmer[farmer].coord.x + 16;
			y = server_state.pp.farmer[farmer].coord.y - 16;
			break;		
	}
	fprintf(stderr, "We're %i\n", farmer);

	for (i = 0; i < FARMER_COUNT; i++) {
		if (server_state.pp.farmer[i].coord.x + 16 <= x)
			continue;
		if (server_state.pp.farmer[i].coord.x >= x + 16)
			continue;
		if (server_state.pp.farmer[i].coord.y + 16 <= y)
			continue;
		if (server_state.pp.farmer[i].coord.y >= y + 16)
			continue;
		return i;
	}

	return -1;
}


void farmer_move_loop(int farmer) {
	int move = 1;

	while (move) {
		move = 0;
		if (ABS(server_state.plist[farmer].dx) >= 1000) {
			if (!sheep_test_collide(server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000, server_state.plist[farmer].dx < 0 ? -1 : 1, 0, 0))
				server_state.plist[farmer].x += (server_state.plist[farmer].dx < 0) ? -1000 : 1000;
			server_state.plist[farmer].dx += (server_state.plist[farmer].dx < 0) ? 1000 : -1000;
			move = 1;
				
		}
			
		if (ABS(server_state.plist[farmer].dy) >= 1000) {
			if (!sheep_test_collide(server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000, 0, server_state.plist[farmer].dy < 0 ? -1 : 1, 0)) {
				server_state.plist[farmer].y += (server_state.plist[farmer].dy < 0) ? -1000 : 1000;
			}
			server_state.plist[farmer].dy += (server_state.plist[farmer].dy < 0) ? 1000 : -1000;
			move = 1;
		}
			d_bbox_move(bbox, i, server_state.plist[i].x / 1000, server_state.plist[i].y / 1000);
	}

	return;
}


void farmer_stab(int src) {
	int target;
	target = farmer_get_farmer_facing(src);

	fprintf(stderr, "Found %i\n", target);
	return;
}
	

void farmer_move() {
	int i, nx, ny, xd, yd, angle;

	for (i = 0; i < FARMER_COUNT; i++) {
		xd = -1 * server_state.plist[i].left + server_state.plist[i].right;
		yd = -1 * server_state.plist[i].up + server_state.plist[i].down;

		nx = d_last_frame_time() * FARMER_SPEED * xd;
		ny = d_last_frame_time() * FARMER_SPEED * yd;
		
		if (xd || yd) {
			angle = conv_array[xd + 1][yd + 1];
			if (angle >= 0) {
				angle <<= 1;
				server_state.pp.farmer[i].coord.dir = angle;
			}
		}

		server_state.pp.farmer[i].coord.moving = (xd || yd);

		/* TODO: Check collision */

		server_state.plist[i].dx += nx;
		server_state.plist[i].dy += ny;
		farmer_move_loop(i);
		server_state.pp.farmer[i].coord.x = server_state.plist[i].x / 1000;
		server_state.pp.farmer[i].coord.y = server_state.plist[i].y / 1000;

		if (server_state.pp.farmer[i].action_yell)
			sheep_panic(server_state.pp.farmer[i].coord.x + 8, server_state.pp.farmer[i].coord.y + 8);
		if (server_state.pp.farmer[i].action_stab)
			farmer_stab(i);
	}

	return;
}


