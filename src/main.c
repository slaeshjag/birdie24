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
	.game_state = GAME_STATE_MENU,
};

static void init_res() {
	DARNIT_INPUT_MAP keymap = {
		.up = TPW_KEY_UP,
		.down = TPW_KEY_DOWN,
		.left = TPW_KEY_LEFT,
		.right = TPW_KEY_RIGHT,
		.x = TPW_KEY_SPACE,
		.y = TPW_KEY_LCTRL,
		.a = TPW_KEY_z,
		.b = TPW_KEY_x,
		.start = TPW_KEY_RETURN,
		.select = TPW_KEY_ESCAPE,
		.l = TPW_KEY_q,
		.r = TPW_KEY_w,
	};
	
	config.font_std = d_font_load("res/DejaVuSans.ttf", 24, 256, 256);
	config.font_big = d_font_load("res/DejaVuSans.ttf", 64, 256, 256);
	config.tileset = d_render_tilesheet_load("res/tiles.png", 16, 16, DARNIT_PFORMAT_RGBA8);
	config.menu_background = d_render_tilesheet_load("res/menu.png", 800, 600, DARNIT_PFORMAT_RGBA8);
	config.spriteset = d_render_tilesheet_load("res/sprites.png", 16, 16, DARNIT_PFORMAT_RGBA8);
	config.map = d_map_load("res/main.ldmz");
	config.music_hillbilly = d_sound_streamed_load("res/hillbilly.ogg", DARNIT_AUDIO_STREAM, DARNIT_AUDIO_STEREO);
	config.platform = d_platform_get();

	d_keymapping_set(keymap);
	sheep_prepare();
	farmer_prepare();

}

void game_state(enum GameState new_state) {
	DARNIT_KEYS keys;
	/*Destructors*/
	switch(config.game_state) {
		case GAME_STATE_GAME:
			d_sound_stop_all();
			break;
		default:
			break;
	}
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	redo:
	config.game_state = new_state;
	/*Contructors*/
	switch(config.game_state) {
		case GAME_STATE_LOBBY_PLAYERNAME:
			lobby_playername_wait();
			break;
		case GAME_STATE_LOBBY:
			server_init();
			if(network_init(PORT) < 0) {
				printf("nooooes\n");
				new_state = GAME_STATE_MENU;
				goto redo;
			}
			client_init();
			break;
		case GAME_STATE_LOBBY_HOST:
			strcpy(config.server.name, config.player_name);
			server_start(config.server.name);
			while(!config.server.connected)
				server_loop();
			printf("connected.\n");
			break;
		case GAME_STATE_GAME:
			d_sound_play(config.music_hillbilly, true, 127, 127, 0);
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
	game_init();
	menu_init();
	
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
