#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <darnit/darnit.h>

#include "main.h"
#include "game.h"
#include "server.h"
#include "sheep.h"

struct Game game;

void game_init() {
	int i;
	
	for(i = 0; i < SHEEP_COUNT; i++) {
		game.sheep.sprite[i] = d_sprite_new(config.spriteset);
		d_sprite_frame_entry_set(game.sheep.sprite[i], 0, 0, 16, 200);
		d_sprite_activate(game.sheep.sprite[i], 0);
		d_sprite_move(game.sheep.sprite[i], 32, 32);
	}
	
	for(i = 0; i < FARMER_COUNT; i++) {
		game.farmer.sprite[i] = d_sprite_new(config.spriteset);
		d_sprite_frame_entry_set(game.farmer.sprite[i], 0, 0, 4*i + 0, 200);
		
		d_sprite_frame_entry_set(game.farmer.sprite[i], 1, 0, 4*i + 1, 200);
		d_sprite_frame_entry_set(game.farmer.sprite[i], 1, 1, 4*i + 2, 200);
		
		d_sprite_frame_entry_set(game.farmer.sprite[i], 2, 0, 4*i + 3, 200);
		d_sprite_frame_entry_set(game.farmer.sprite[i], 2, 1, 4*i + 0, 200);
		d_sprite_activate(game.farmer.sprite[i], 0);
		d_sprite_move(game.farmer.sprite[i], 32, 32);
		
		game.farmer.sprite_pitchfork[i] = d_sprite_new(config.spriteset);
		d_sprite_frame_entry_set(game.farmer.sprite_pitchfork[i], 0, 0, 19, 150);
		d_sprite_frame_entry_set(game.farmer.sprite_pitchfork[i], 0, 1, 20, 150);
		d_sprite_activate(game.farmer.sprite_pitchfork[i], 0);
		d_sprite_animate_start(game.farmer.sprite_pitchfork[i]);
		
		game.farmer.sprite_yell[i] = d_sprite_new(config.spriteset);
		d_sprite_frame_entry_set(game.farmer.sprite_yell[i], 0, 0, 17, 100);
		d_sprite_frame_entry_set(game.farmer.sprite_yell[i], 0, 1, 18, 100);
		d_sprite_activate(game.farmer.sprite_yell[i], 0);
		d_sprite_animate_start(game.farmer.sprite_yell[i]);
		
		
	}
	
	game.text = d_text_surface_new(config.font_std, 256, config.platform.screen_w, 16, config.platform.screen_h - 32);
}

void game_loop() {
	static int tick = 0;
	int i;
	struct proto_control_packet packet = {};
	char tmp[64];
	DARNIT_KEYS keys;
	
	keys = d_keys_get();
	
	if(keys.up) {
		packet.up = 1;
	} else if(keys.down) {
		packet.down = 1;
	}
	
	if(keys.left) {
		packet.left = 1;
	} else if(keys.right) {
		packet.right = 1;
	}
	
	if(keys.x) {
		packet.stab = 1;
	} else if(keys.y) {
		packet.yell = 1;
	}
	
	if(keys.select) {
		d_keys_set(keys);
		game_state(GAME_STATE_MENU);
		return;
	}
	
	server_loop();
	
	d_tilemap_draw(config.map->layer[0].tilemap);
	d_tilemap_draw(config.map->layer[1].tilemap);
	for(i = 0; i < SHEEP_COUNT; i++) {
		d_sprite_move(game.sheep.sprite[i], game.sheep.sheep[i].x, game.sheep.sheep[i].y);
		d_sprite_rotate(game.sheep.sprite[i], game.sheep.sheep[i].angle);
		d_sprite_draw(game.sheep.sprite[i]);
	}
	
	d_text_surface_reset(game.text);
	sprintf(tmp, "Timer: %i", config.timer);
	d_text_surface_string_append(game.text, tmp);
	for(i = 0; i < FARMER_COUNT; i++) {
		sprintf(tmp, "    Scrore player %i: %i", i + 1, config.points[i]);
		d_text_surface_string_append(game.text, tmp);
		d_sprite_move(game.farmer.sprite[i], game.farmer.farmer[i].x, game.farmer.farmer[i].y);
		d_sprite_rotate(game.farmer.sprite[i], game.farmer.farmer[i].angle);
		
		if(game.farmer.farmer[i].move) {
			d_sprite_direction_set(game.farmer.sprite[i], 1);
			d_sprite_animate_start(game.farmer.sprite[i]);
		} else {
			d_sprite_frame_set(game.farmer.sprite[i], 0);
			d_sprite_direction_set(game.farmer.sprite[i], 0);
			d_sprite_animate_stop(game.farmer.sprite[i]);
		}
		d_sprite_draw(game.farmer.sprite[i]);
		
		if(game.farmer.farmer[i].stab) {
			d_sprite_move(game.farmer.sprite_pitchfork[i], game.farmer.farmer[i].x + 10.0*cos(M_PI*game.farmer.farmer[i].angle/1800) + 5.0*sin(M_PI*game.farmer.farmer[i].angle/1800), game.farmer.farmer[i].y - 10.0*sin(M_PI*game.farmer.farmer[i].angle/1800) + 5.0*cos(M_PI*game.farmer.farmer[i].angle/1800));
			d_sprite_rotate(game.farmer.sprite_pitchfork[i], game.farmer.farmer[i].angle);
			d_sprite_draw(game.farmer.sprite_pitchfork[i]);
			
		}
		
		if(game.farmer.farmer[i].yell) {
			d_sprite_move(game.farmer.sprite_yell[i], game.farmer.farmer[i].x + 16, game.farmer.farmer[i].y - 16);
			d_sprite_draw(game.farmer.sprite_yell[i]);
		}
	}
	
	if(!tick) {
		packet.type = PROTO_TYPE_CONTROL;
		network_send(config.server.addr, &packet, sizeof(struct proto_control_packet));
	}
	d_text_surface_draw(game.text);
	tick++;
	tick %= 3;
}
