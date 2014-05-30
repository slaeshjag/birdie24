#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <darnit/darnit.h>

#include "game.h"
#include "server.h"
#include "sheep.h"

void game_init() {
	int i;
	
	for(i = 0; i < SHEEP_COUNT; i++) {
		game.sheep.sprite[i] = d_sprite_new(config.tileset);
		d_sprite_frame_entry_set(game.sheep.sprite[i], 0, 0, 6, 200);
	}
}

void game_loop() {
	server_loop();
	for(i = 0; i < SHEEP_COUNT; i++) {
		d_sprite_draw(game.sheep.sprite[i]);
	}
}
