#include "farmer.h"
#include "server.h"
#include "main.h"
#include <darnit/darnit.h>

static int conv_array[3][3] = {
	{ 5, 6, 7 }, { 4, -1, 0 }, { 3, 2, 1 }
};


void farmer_spawn() {
	int i;

	for (i = 0; i < FARMER_COUNT; i++) {
		server_state.plist[i].x = config.platform.screen_w / 2;
		server_state.plist[i].y = config.platform.screen_h / FARMER_COUNT * i;
	}

	farmer_move();
	return;
}


void farmer_move() {
	int i, nx, ny, xd, yd, angle;

	for (i = 0; i < FARMER_COUNT; i++) {
		nx = server_state.plist[i].x;
		ny = server_state.plist[i].y;
		xd = -1 * server_state.plist[i].left + server_state.plist[i].right;
		yd = -1 * server_state.plist[i].right + server_state.plist[i].down;

		nx += d_last_frame_time() * FARMER_SPEED * xd;
		ny += d_last_frame_time() * FARMER_SPEED * yd;
		
		if (xd || yd) {
			angle = conv_array[xd + 1][yd + 1];
			if (angle >= 0) {
				angle <<= 1;
				server_state.pp.farmer[i].coord.dir = angle;
			}
		}

		server_state.pp.farmer[i].coord.moving = (xd || yd);

		/* TODO: Check collision */

		nx = (nx < 0) ? 0 : nx;
		ny = (ny < 0) ? 0 : ny;

		server_state.pp.farmer[i].coord.x = nx / 1000;
		server_state.pp.farmer[i].coord.y = ny / 1000;
	}

	return;
}


