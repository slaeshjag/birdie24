#include "farmer.h"
#include "server.h"
#include "main.h"
#include <darnit/darnit.h>

static int conv_array[3][3] = {
	{ 5, 6, 7 }, { 4, -1, 0 }, { 3, 2, 1 }
};

#define	ABS(x)		((x) < 0 ? -(x) : (x))

void farmer_spawn() {
	server_state.plist[0].x = 0;
	server_state.plist[0].y = config.platform.screen_h / 2 * 1000 + 4000;
	server_state.plist[0].dx = FARMER_INTRO_SPEED;
	server_state.plist[0].dy = 0;
	server_state.pp.farmer[0].coord.dir = 4;
	
	if(FARMER_COUNT >= 2) {
		server_state.plist[1].x = config.platform.screen_w - 16000;
		server_state.plist[1].y = config.platform.screen_h / 2 * 1000 + 4000;
		server_state.plist[1].dx = -FARMER_INTRO_SPEED;
		server_state.plist[1].dy = 0;
		server_state.pp.farmer[1].coord.dir = 12;
	}
	
	if(FARMER_COUNT >= 3) {
		server_state.plist[2].x = config.platform.screen_w / 2 * 1000 + 4000;
		server_state.plist[2].y = 0;
		server_state.plist[2].dx = 0;
		server_state.plist[2].dy = FARMER_INTRO_SPEED;
		server_state.pp.farmer[2].coord.dir = 8;
	}
	
	if(FARMER_COUNT >= 4) {
		server_state.plist[3].x = config.platform.screen_w / 2 * 1000 + 4000;
		server_state.plist[3].y = config.platform.screen_h - 16000;
		server_state.plist[3].dx = 0;
		server_state.plist[3].dy = -FARMER_INTRO_SPEED;
		server_state.pp.farmer[3].coord.dir = 0;
	}
	
	farmer_move();
	return;
}

void farmer_intro_set_speed() {
	server_state.plist[0].dx += FARMER_INTRO_SPEED;
	server_state.plist[0].dy = 0;
	
	if(FARMER_COUNT >= 2) {
		server_state.plist[1].dx += -FARMER_INTRO_SPEED;
		server_state.plist[1].dy = 0;
	}
	
	if(FARMER_COUNT >= 3) {
		server_state.plist[2].dx = 0;
		server_state.plist[2].dy += FARMER_INTRO_SPEED;
	}
	
	if(FARMER_COUNT >= 4) {
		server_state.plist[3].dx = 0;
		server_state.plist[3].dy += -FARMER_INTRO_SPEED;
	}
}

void farmer_move_loop(int farmer) {
	int move = 1;

	while (move) {
		move = 0;
		if (ABS(server_state.plist[farmer].dx) >= 1000) {
			server_state.plist[farmer].x += (server_state.plist[farmer].dx < 0) ? -1000 : 1000;
			server_state.plist[farmer].dx += (server_state.plist[farmer].dx < 0) ? 1000 : -1000;
			move = 1;
			if (sheep_test_collide(server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000))
				server_state.plist[farmer].x -= (server_state.plist[farmer].dx < 0) ? -1000 : 1000;
				
		}
			
		if (ABS(server_state.plist[farmer].dy) >= 1000) {
			server_state.plist[farmer].y += (server_state.plist[farmer].dy < 0) ? -1000 : 1000;
			server_state.plist[farmer].dy += (server_state.plist[farmer].dy < 0) ? 1000 : -1000;
			move = 1;
			if (sheep_test_collide(server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000))
				server_state.plist[farmer].y -= (server_state.plist[farmer].dy < 0) ? -1000 : 1000;
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

		server_state.plist[i].dx = nx;
		server_state.plist[i].dy = ny;
		farmer_move_loop(i);
		server_state.pp.farmer[i].coord.x = server_state.plist[i].x / 1000;
		server_state.pp.farmer[i].coord.y = server_state.plist[i].y / 1000;
	}

	return;
}

void farmer_intro() {
	int i;
	if(server_state.intro_time < 90) {
		farmer_intro_set_speed();
		for (i = 0; i < FARMER_COUNT; i++) {
			farmer_move_loop(i);
			server_state.pp.farmer[i].coord.x = server_state.plist[i].x / 1000;
			server_state.pp.farmer[i].coord.y = server_state.plist[i].y / 1000;
		}
	} else {
		for (i = 0; i < FARMER_COUNT; i++) {
			server_state.pp.farmer[i].action_stab = 1;
			server_state.pp.farmer[i].action_yell = 1;
		}
	}
}

