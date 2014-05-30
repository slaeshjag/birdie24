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
#include "server.h"
#include "client.h"
#include "network.h"

static void (*state_loop[GAME_STATES])() = {
	[GAME_STATE_MENU] = menu_loop,
	[GAME_STATE_LOBBY_PLAYERNAME] = lobby_playername_loop,
	[GAME_STATE_LOBBY] = lobby_loop,
	[GAME_STATE_LOBBY_HOST] = lobby_host_loop,
	[GAME_STATE_LOBBY_JOIN] = lobby_join_loop,
	[GAME_STATE_GAME] = game_loop,
};

struct Config config = {
	.game_state = GAME_STATE_LOBBY_PLAYERNAME,
};

static void init_res() {
	config.font_std = d_font_load("res/DejaVuSans.ttf", 24, 256, 256);
	config.platform = d_platform_get();
}

void game_state(enum GameState new_state) {
	DARNIT_KEYS keys;
	/*Destructors*/
	switch(config.game_state) {
		default:
			break;
	}
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	redo:
	config.game_state = new_state;
	/*Contructors*/
	switch(config.game_state) {
		case GAME_STATE_LOBBY_HOST:
			if(network_init(PORT) < 0) {
				printf("nooooes\n");
				new_state = GAME_STATE_LOBBY;
				goto redo;
			}
			client_init();
			strcpy(config.server.name, "arne");
			server_start("arne");
			while(!config.server.connected)
				server_loop();
			printf("connected.\n");
			break;
		default:
			break;
	}
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
