#ifndef __MAIN_H_
#define __MAIN_H_
#include <darnit/darnit.h>
#include "proto.h"
#include "stdbool.h"

enum GameState {
	GAME_STATE_MENU,
	GAME_STATE_LOBBY_PLAYERNAME,
	GAME_STATE_LOBBY,
	GAME_STATE_LOBBY_HOST,
	GAME_STATE_LOBBY_JOIN,
	GAME_STATE_GAME,
	
	GAME_STATES,
};

struct Config {
	enum GameState game_state;
	
	DARNIT_PLATFORM platform;
	
	DARNIT_FONT *font_std;
	DARNIT_FONT *font_big;
	DARNIT_TILESHEET *tileset;
	DARNIT_TILESHEET *spriteset;
	DARNIT_TILESHEET *menu_background;
	DARNIT_TILESHEET *tractor;
	DARNIT_SOUND *music_hillbilly;
	DARNIT_MAP *map;
	
	char *text_aux;
	
	char player_name[PROTO_PLAYER_NAME];
	
	struct {
		char player[FARMER_COUNT][PROTO_PLAYER_NAME];
		int players;
	} player;
	
	struct {
		unsigned long addr;
		char name[PROTO_GAME_NAME_MAX];
		bool connected;
	} server;
	
	int points[FARMER_COUNT];
	int timer;
	
	int music_handle;
} extern config;

struct List {
	void *value;
	struct List *next;
};

void game_state(enum GameState new_state);

#endif
