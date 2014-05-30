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
	DARNIT_TEXT_SURFACE *text_help;
} static menu;

void menu_init() {
	menu.text_title = d_text_surface_new(config.font_big, 32, config.platform.screen_w, config.platform.screen_w/2 - 256, 64);
	d_text_surface_string_append(menu.text_title, "BondeN^wArs™");
	menu.text_subtitle = d_text_surface_new(config.font_std, 32, config.platform.screen_w, config.platform.screen_w/2 - 110, 128);
	d_text_surface_string_append(menu.text_subtitle, "~Press enter~");
	menu.text_help = d_text_surface_new(config.font_std, 128, config.platform.screen_w, 16, config.platform.screen_h - 32);
	d_text_surface_string_append(menu.text_help, "Arrows: move, Ctrl: shout curses, Space: use pitchfork");
}

void menu_loop() {
	DARNIT_KEYS keys;
	
	d_render_tile_blit(config.menu_background, 0, 0, 0);
	d_text_surface_draw(menu.text_title);
	d_text_surface_draw(menu.text_subtitle);
	d_text_surface_draw(menu.text_help);
	
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(keys.select) {
		d_quit();
	}
	
	if(keys.start) {
		game_state(GAME_STATE_LOBBY_PLAYERNAME);
	}
}
