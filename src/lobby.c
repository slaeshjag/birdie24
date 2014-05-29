#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <darnit/darnit.h>

#include "main.h"
#include "proto.h"

static struct {
	DARNIT_TEXT_SURFACE *text_playername;
	DARNIT_MENU *inputfield_playername;
} lobby_playername;

static struct {
	DARNIT_LINE *line;
	DARNIT_TEXT_SURFACE *list_players;
	DARNIT_TEXT_SURFACE *list_games;

	int selected_player;
	int selected_game;
} lobby;

static const char *text_host = "[Host game]";

static void update_lists(const char **player, int players, const char **game, int games) {
	int i;
	
	d_text_surface_reset(lobby.list_players);
	d_text_surface_reset(lobby.list_games);
	
	d_text_surface_color_next(lobby.list_players, 255, 255, 255);
	d_text_surface_string_append(lobby.list_players, "Players\n\n");
	d_text_surface_color_next(lobby.list_games, 255, 255, 255);
	d_text_surface_string_append(lobby.list_games, "Games\n\n");
	
	for(i = 0; i < players; i++) {
		/*if(i == lobby.selected_player) {
			d_text_surface_color_next(lobby.list_players, 255, 0, 0);
		} else {*/
			d_text_surface_color_next(lobby.list_players, 255, 255, 255);
		/*}*/
		d_text_surface_string_append(lobby.list_players, player[i]);
	}
	
	for(i = 0; i < games; i++) {
		if(i == lobby.selected_game) {
			d_text_surface_color_next(lobby.list_games, 255, 0, 0);
		} else {
			d_text_surface_color_next(lobby.list_games, 255, 255, 255);
		}
		d_text_surface_string_append(lobby.list_games, game[i]);
	}
}

void lobby_init() {
	//TODO: first item in game list is [Host game]
	lobby.line = d_render_line_new(1, 2);
	d_render_line_move(lobby.line, 0, config.platform.screen_w/2, 0, config.platform.screen_w/2, config.platform.screen_h);
	
	lobby.list_players = d_text_surface_color_new(config.font_std, 2048, 300, 64, 0);
	lobby.list_games = d_text_surface_color_new(config.font_std, 2048, 300, config.platform.screen_w/2 + 64, 0);
	
	update_lists(NULL, 0, &text_host, 1);
	
	lobby_playername.text_playername = d_text_surface_new(config.font_std, 64, 1024, 64, 64);
	lobby_playername.inputfield_playername = d_menu_textinput_new(64, 128, config.font_std, config.player_name, PROTO_PLAYER_NAME - 1, config.platform.screen_w);
	
	d_text_surface_string_append(lobby_playername.text_playername, "Enter your player name:");
}

void lobby_playername_loop() {
	d_text_surface_draw(lobby_playername.text_playername);
	//printf("arne %i\n", d_menu_loop(lobby_playername.inputfield_playername));
	switch(d_menu_loop(lobby_playername.inputfield_playername)) {
		case -1:
			break;
		case 0:
		case -2:
			game_state(GAME_STATE_MENU);
			break;
		default:
			game_state(GAME_STATE_LOBBY);
			break;
	}
}

void lobby_loop() {
	DARNIT_KEYS keys;
	d_render_line_draw(lobby.line, 1);
	d_text_surface_draw(lobby.list_players);
	d_text_surface_draw(lobby.list_games);
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(keys.up) {
		if(lobby.selected_game > 0)
			lobby.selected_game--;
	} else if(keys.down) {
		//TODO: fiiiiiiiix
		if(lobby.selected_game < 9000)
			lobby.selected_game++;
	}
	
	if(keys.start) {
		if(lobby.selected_game == 0) {
			game_state(GAME_STATE_LOBBY_HOST);
		} else {
			game_state(GAME_STATE_LOBBY_JOIN);
		}
	}
}

void lobby_host_loop() {
	server_loop();
}

void lobby_join_loop() {
	
}
