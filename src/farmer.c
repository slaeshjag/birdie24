#include "farmer.h"
#include "server.h"
#include "main.h"
#include <darnit/darnit.h>

static int conv_array[3][3] = {
	{ 5, 6, 7 }, { 4, -1, 0 }, { 3, 2, 1 }
};

#define	ABS(x)		((x) < 0 ? -(x) : (x))

void farmer_spawn() {
	int i;

	for (i = 0; i < FARMER_COUNT; i++) {
		server_state.plist[i].x = config.platform.screen_w / 2 * 1000;
		server_state.plist[i].y = config.platform.screen_h / FARMER_COUNT * i * 1000 + 1000;
		server_state.plist[i].dx = server_state.plist[i].dy = 0;
	}

	farmer_move();
	return;
}


void farmer_move_loop(int farmer) {
	int move = 1;

	while (move) {
		move = 0;
		if (ABS(server_state.plist[farmer].dx) >= 1000) {
			if (!sheep_test_collide(server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000, server_state.plist[farmer].dx < 0 ? -1 : 1, 0))
				server_state.plist[farmer].x += (server_state.plist[farmer].dx < 0) ? -1000 : 1000;
			server_state.plist[farmer].dx += (server_state.plist[farmer].dx < 0) ? 1000 : -1000;
			move = 1;
				
		}
			
		if (ABS(server_state.plist[farmer].dy) >= 1000) {
			if (!sheep_test_collide(server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000, 0, server_state.plist[farmer].dy < 0 ? -1 : 1)) {
				server_state.plist[farmer].y += (server_state.plist[farmer].dy < 0) ? -1000 : 1000;
			}
			server_state.plist[farmer].dy += (server_state.plist[farmer].dy < 0) ? 1000 : -1000;
			move = 1;
		}
//			d_bbox_move(bbox, i, server_state.sheep[i].x / 1000, server_state.sheep[i].y / 1000);
	}

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
	}

	return;
}


