#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <darnit/darnit.h>

#include "lobby.h"
#include "main.h"
#include "proto.h"
#include "server.h"

struct LobbyClient lobby_client;

static struct {
	DARNIT_TEXT_SURFACE *text_playername;
	DARNIT_MENU *inputfield_playername;
} lobby_playername;

static struct {
	DARNIT_TEXT_SURFACE *start_game;
} lobby_host;

static struct {
	DARNIT_TEXT_SURFACE *list_players;
} lobby_join;

static struct {
	DARNIT_LINE *line;
	DARNIT_TEXT_SURFACE *list_players;
	DARNIT_TEXT_SURFACE *list_games;

	int selected_player;
	int selected_game;
} lobby;

static void update_lists(struct List *player, struct List *game) {
	int i;
	
	d_text_surface_reset(lobby.list_players);
	d_text_surface_reset(lobby.list_games);
	
	d_text_surface_color_next(lobby.list_players, 255, 255, 255);
	d_text_surface_string_append(lobby.list_players, "Players\n\n");
	d_text_surface_color_next(lobby.list_games, 255, 255, 255);
	d_text_surface_string_append(lobby.list_games, "Games\n\n");
	
	#if 0
	for(i = 0; i < players; i++) {
		/*if(i == lobby.selected_player) {
			d_text_surface_color_next(lobby.list_players, 255, 0, 0);
		} else {*/
			d_text_surface_color_next(lobby.list_players, 255, 255, 255);
		/*}*/
		d_text_surface_string_append(lobby.list_players, player[i]);
	}
	#endif
	
	for(i = 0; game; i++, game = game->next) {
		if(i == lobby.selected_game) {
			d_text_surface_color_next(lobby.list_games, 255, 0, 0);
		} else {
			d_text_surface_color_next(lobby.list_games, 255, 255, 255);
		}
		d_text_surface_string_append(lobby.list_games, ((struct LobbyHost *) (game->value))->name);
		d_text_surface_string_append(lobby.list_games, "'s game\n");
	}
}

static void update_player_list(DARNIT_TEXT_SURFACE *surface, char player[][PROTO_PLAYER_NAME]) {
	int i;
	
	d_text_surface_reset(surface);
	
	d_text_surface_color_next(surface, 255, 255, 255);
	d_text_surface_string_append(surface, "Players\n\n");
	
	for(i = 0; i < FARMER_COUNT; i++) {
		if(!player[i][0])
			continue;
		d_text_surface_color_next(surface, 255, 255, 255);
		d_text_surface_string_append(surface, player[i]);
		d_text_surface_char_append(surface, "\n");
	}
}

void lobby_init() {
	//TODO: first item in game list is [Host game]
	lobby.line = d_render_line_new(1, 2);
	d_render_line_move(lobby.line, 0, config.platform.screen_w/2, 0, config.platform.screen_w/2, config.platform.screen_h);
	
	lobby.list_players = d_text_surface_color_new(config.font_std, 2048, 300, 64, 0);
	lobby.list_games = d_text_surface_color_new(config.font_std, 2048, 300, config.platform.screen_w/2 + 64, 0);
	
	lobby_join.list_players = d_text_surface_color_new(config.font_std, 2048, 300, 64, 0);
	lobby_host.start_game = d_text_surface_color_new(config.font_std, 32, 300, 64, config.platform.screen_h - 96);
	d_text_surface_color_next(lobby_host.start_game, 255, 0, 0);
	d_text_surface_string_append(lobby_host.start_game, "Start game");
	
	update_lists(NULL, lobby_client.client);
	
	lobby_playername.text_playername = d_text_surface_new(config.font_std, 64, 1024, 64, 64);
	lobby_playername.inputfield_playername = d_menu_textinput_new(64, 128, config.font_std, config.player_name, PROTO_PLAYER_NAME - 1, config.platform.screen_w);
	
	d_text_surface_string_append(lobby_playername.text_playername, "Enter your player name:");
	
	lobby.selected_game = 0;
	lobby.selected_player = 0;
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
	static int tick = 0;
	DARNIT_KEYS keys;
	int i;
	struct List *game = lobby_client.client;
	
	d_render_line_draw(lobby.line, 1);
	d_text_surface_draw(lobby.list_players);
	d_text_surface_draw(lobby.list_games);
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(!tick) {
		update_lists(NULL, lobby_client.client);
	}
	
	if(keys.up) {
		if(lobby.selected_game > 0)
			lobby.selected_game--;
	} else if(keys.down) {
		if(lobby.selected_game < lobby_client.clients)
			lobby.selected_game++;
	}
	
	if(keys.start) {
		if(lobby.selected_game == 0) {
			game_state(GAME_STATE_LOBBY_HOST);
		} else {
			//TODO: join
			for(i = 0; game; i++, game = game->next) {
				if(i == lobby.selected_game) {
					config.server.addr = ((struct LobbyHost *) (game->value))->addr;
					goto join;
				}
			}
			game_state(GAME_STATE_LOBBY_HOST);
			return;
			join:
			game_state(GAME_STATE_LOBBY_JOIN);
		}
	}
	tick++;
	tick %= 3;
}

void lobby_join_loop() {
	struct proto_join_packet packet;
	static int tick = 0;
	DARNIT_KEYS keys;
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(!tick) {
		packet.type = PROTO_TYPE_JOIN;
		packet.player_id = -1;
		strcpy(packet.player_name, config.player_name);
		network_send(config.server.addr, &packet, sizeof(struct proto_join_packet));
		update_player_list(lobby_join.list_players, config.player.player);
	}
	
	if(keys.select) {
		game_state(GAME_STATE_LOBBY);
	}
	
	d_text_surface_draw(lobby_join.list_players);
	tick++;
	tick %= 3;
}

void lobby_host_loop() {
	struct proto_join_packet packet;
	static int tick = 0;
	DARNIT_KEYS keys;
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(!tick) {
		packet.type = PROTO_TYPE_JOIN;
		packet.player_id = -1;
		strcpy(packet.player_name, config.player_name);
		network_send(config.server.addr, &packet, sizeof(struct proto_join_packet));
		update_player_list(lobby_join.list_players, config.player.player);
	}
	server_loop();
	
	if(keys.select) {
		game_state(GAME_STATE_LOBBY);
	}
	if(keys.start) {
		game_state(GAME_STATE_GAME);
	}
	
	d_text_surface_draw(lobby_join.list_players);
	d_text_surface_draw(lobby_host.start_game);
	tick++;
	tick %= 3;
}
