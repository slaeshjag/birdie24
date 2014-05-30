#ifndef __GAME_H_
#define __GAME_H_

#include "sheep.h"

struct Game {
	struct {
		struct Sheep sheep[SHEEP_COUNT];
		DARNIT_SPRITE *sprite[SHEEP_COUNT];
	} sheep;
	struct {
		struct Farmer farmer[FARMER_COUNT];
		DARNIT_SPRITE *sprite[FARMER_COUNT];
		DARNIT_SPRITE *sprite_pitchfork[FARMER_COUNT];
		DARNIT_SPRITE *sprite_yell[FARMER_COUNT];
	} farmer;
} extern game;

void game_init();
void game_loop();

#endif
