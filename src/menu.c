#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <darnit/darnit.h>

#include "menu.h"
#include "main.h"

struct {
	DARNIT_TEXT_SURFACE *text_title;
	DARNIT_TEXT_SURFACE *text_subtitle;
} static menu;

void menu_init() {
	menu.text_title = d_text_surface_new(config.font_big, 32, config.platform.screen_w, config.platform.screen_w/2 - 256, 64);
	d_text_surface_string_append(menu.text_title, "BondeN^wArs™");
	menu.text_subtitle = d_text_surface_new(config.font_std, 32, config.platform.screen_w, config.platform.screen_w/2 - 110, 128);
	d_text_surface_string_append(menu.text_subtitle, "~Press enter~");
}

void menu_loop() {
	DARNIT_KEYS keys;
	
	d_render_tile_blit(config.menu_background, 0, 0, 0);
	d_text_surface_draw(menu.text_title);
	d_text_surface_draw(menu.text_subtitle);
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(keys.select) {
		d_quit();
	}
	
	if(keys.start) {
		game_state(GAME_STATE_LOBBY_PLAYERNAME);
	}
}
