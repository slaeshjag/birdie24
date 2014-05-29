#ifndef __MAIN_H_
#define __MAIN_H_

struct Config {
	enum GameState {
		GAME_STATE_MENU,
		GAME_STATE_GAME,
		
		GAME_STATES,
	} game_state;
};

#endif
