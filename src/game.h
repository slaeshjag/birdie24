#ifndef __GAME_H_
#define __GAME_H_

#include "sheep.h"

struct Game {
	struct {
		struct Sheep sheep[SHEEP_COUNT];
		DARNIT_SPRITE *sprite[SHEEP_COUNT];
	} sheep;
} extern game;

void game_loop();

#endif
