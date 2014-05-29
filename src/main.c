#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <darnit/darnit.h>

#include "main.h"
#include "menu.h"
#include "game.h"
#include "proto.h"
#include "lobby.h"

static void (*state_loop[GAME_STATES])() = {
	[GAME_STATE_MENU] = menu_loop,
	[GAME_STATE_LOBBY_PLAYERNAME] = lobby_playername_loop,
	[GAME_STATE_LOBBY] = lobby_loop,
	[GAME_STATE_GAME] = game_loop,
};

struct Config config = {
	.game_state = GAME_STATE_LOBBY_PLAYERNAME,
};

void init_res() {
	config.font_std = d_font_load("res/DejaVuSans.ttf", 24, 256, 256);
}

int main(int argc, char **argv) {
	d_init_custom("birdie24", 800, 600, false, "birdie24", NULL);
	init_res();
	proto_init();
	lobby_init();
	
	for(;;) {
		d_render_begin();
		d_render_blend_enable();
		d_render_tint(255, 255, 255, 255);
		state_loop[config.game_state]();
		d_render_end();
		d_loop();
	}
	
	d_quit();
	return 0;
}
