#ifndef __MAIN_H_
#define __MAIN_H_
#include <darnit/darnit.h>
#include "proto.h"

struct Config {
	enum GameState {
		GAME_STATE_MENU,
		GAME_STATE_LOBBY_PLAYERNAME,
		GAME_STATE_LOBBY,
		GAME_STATE_GAME,
		
		GAME_STATES,
	} game_state;
	
	DARNIT_PLATFORM platform;
	
	DARNIT_FONT *font_std;
	char player_name[PROTO_PLAYER_NAME];
} extern config;

#endif
