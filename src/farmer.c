#include "farmer.h"
#include "server.h"
#include "main.h"
#include <darnit/darnit.h>

static int conv_array[3][3] = {
	{ 5, 6, 7 }, { 4, -1, 0 }, { 3, 2, 1 }
};

#define	ABS(x)		((x) < 0 ? -(x) : (x))

static DARNIT_BBOX *bbox;

static int farmer_stunn_x_vel[8] = { 0, 141, 200, 141, 0, -141, -200, -141 };
static int farmer_stunn_y_vel[8] = { 200, 141, 0, -141, -200, -141, 0, 141 };
void farmer_prepare() {
	bbox = d_bbox_new(FARMER_COUNT);
	d_bbox_set_indexkey(bbox);
}


int farmer_get_farmer_facing(int farmer) {
	int x, y, i;
	unsigned int p;
	x = y = 0;
	switch (server_state.pp.farmer[farmer].coord.dir >> 1) {
		case 0:
			x = server_state.pp.farmer[farmer].coord.x;
			y = server_state.pp.farmer[farmer].coord.y + 16;
			break;
		case 1:
			x = server_state.pp.farmer[farmer].coord.x + 16;
			y = server_state.pp.farmer[farmer].coord.y + 16;
			break;
		case 2:
			x = server_state.pp.farmer[farmer].coord.x + 16;
			y = server_state.pp.farmer[farmer].coord.y;
			break;
		case 3:
			x = server_state.pp.farmer[farmer].coord.x + 16;
			y = server_state.pp.farmer[farmer].coord.y - 16;
			break;
		case 4:
			x = server_state.pp.farmer[farmer].coord.x;
			y = server_state.pp.farmer[farmer].coord.y - 16;
			break;
		case 5:
			x = server_state.pp.farmer[farmer].coord.x - 16;
			y = server_state.pp.farmer[farmer].coord.y - 16;
			break;
		case 6:
			x = server_state.pp.farmer[farmer].coord.x - 16;
			y = server_state.pp.farmer[farmer].coord.y;
			break;
		case 7:
			x = server_state.pp.farmer[farmer].coord.x - 16;
			y = server_state.pp.farmer[farmer].coord.y + 16;
			break;
	}

	i = d_bbox_test(bbox, x, y, 16, 16, &p, 1);
	if (i)
		return p;
	return -1;
}


void farmer_spawn() {
	server_state.plist[0].x = 0;
	server_state.plist[0].y = config.platform.screen_h / 2 * 1000 + 4000;
	server_state.plist[0].dx = FARMER_INTRO_SPEED;
	server_state.plist[0].dy = 0;
	server_state.pp.farmer[0].coord.dir = 4;
	d_bbox_add(bbox, 0, 0, 16, 16);
	
	if(FARMER_COUNT >= 2) {
		server_state.plist[1].x = config.platform.screen_w * 1000 - 16000 ;
		server_state.plist[1].y = config.platform.screen_h / 2 * 1000 + 4000;
		server_state.plist[1].dx = -FARMER_INTRO_SPEED;
		server_state.plist[1].dy = 0;
		server_state.pp.farmer[1].coord.dir = 12;
		d_bbox_add(bbox, 0, 0, 16, 16);
	}
	
	if(FARMER_COUNT >= 3) {
		server_state.plist[2].x = config.platform.screen_w / 2 * 1000 + 4000;
		server_state.plist[2].y = 0;
		server_state.plist[2].dx = 0;
		server_state.plist[2].dy = FARMER_INTRO_SPEED;
		server_state.pp.farmer[2].coord.dir = 8;
		d_bbox_add(bbox, 0, 0, 16, 16);
	}
	
	if(FARMER_COUNT >= 4) {
		server_state.plist[3].x = config.platform.screen_w / 2 * 1000 + 4000;
		server_state.plist[3].y = config.platform.screen_h - 16000;
		server_state.plist[3].dx = 0;
		server_state.plist[3].dy = -FARMER_INTRO_SPEED;
		server_state.pp.farmer[3].coord.dir = 0;
		d_bbox_add(bbox, 0, 0, 16, 16);
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
		d_bbox_move(bbox, farmer, server_state.plist[farmer].x / 1000, server_state.plist[farmer].y / 1000);
	}

	return;
}


void farmer_stab(int src) {
	int target;
	target = farmer_get_farmer_facing(src);
	if (target >= 0)
		server_state.plist[target].stab_timeout = FARMER_STAB_TIME;

	return;
}
	

void farmer_move() {
	int i, nx, ny, xd, yd, angle;

	for (i = 0; i < FARMER_COUNT; i++) {
		if (server_state.plist[i].stab_timeout) {
			server_state.plist[i].dx += farmer_stunn_x_vel[server_state.pp.farmer[i].coord.dir >> 1];
			server_state.plist[i].dy += farmer_stunn_y_vel[server_state.pp.farmer[i].coord.dir >> 1];
			server_state.plist[i].stab_timeout--;
			goto update;
		}
			
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

		server_state.plist[i].dx += nx;
		server_state.plist[i].dy += ny;
		server_state.pp.farmer[i].coord.moving = (xd || yd);

		update:
		/* TODO: Check collision */

		server_state.pp.farmer[i].action_stabbed = (server_state.plist[i].stab_timeout > 0);
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

