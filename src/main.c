#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <darnit/darnit.h>

#include "main.h"
#include "menu.h"
#include "game.h"
#include "proto.h"

static void (*state_loop[GAME_STATES])() = {
	menu_loop,
	game_loop,
};

static struct Config config = {
	.game_state = GAME_STATE_MENU,
};

int main(int argc, char **argv) {
	d_init_custom("birdie24", 800, 600, false, "birdie24", NULL);
	proto_init();
	
	for(;;) {
		d_render_begin();
		state_loop[config.game_state]();
		d_render_end();
		d_loop();
	}
	
	d_quit();
	return 0;
}
