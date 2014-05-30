#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
		
		d_sprite_frame_entry_set(game.farmer.sprite[i], 2, 0, 4*i + 0, 200);
		d_sprite_frame_entry_set(game.farmer.sprite[i], 2, 1, 4*i + 3, 200);
		d_sprite_activate(game.farmer.sprite[i], 0);
		
		d_sprite_move(game.farmer.sprite[i], 32, 32);
	}
}

void game_loop() {
	static int tick = 0;
	int i;
	struct proto_control_packet packet = {};
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
		d_sprite_draw(game.sheep.sprite[i]);
	}
	
	for(i = 0; i < FARMER_COUNT; i++) {
		d_sprite_move(game.farmer.sprite[i], game.farmer.farmer[i].x, game.farmer.farmer[i].y);
		d_sprite_rotate(game.farmer.sprite[i], game.farmer.farmer[i].angle);
		if(game.farmer.farmer[i].stab) {
			d_sprite_direction_set(game.farmer.sprite[i], 2);
			d_sprite_animate_start(game.farmer.sprite[i]);
		} else if(game.farmer.farmer[i].move) {
			d_sprite_direction_set(game.farmer.sprite[i], 1);
			d_sprite_animate_start(game.farmer.sprite[i]);
		} else {
			d_sprite_frame_set(game.farmer.sprite[i], 0);
			d_sprite_direction_set(game.farmer.sprite[i], 0);
			d_sprite_animate_stop(game.farmer.sprite[i]);
		}
		d_sprite_draw(game.farmer.sprite[i]);
	}
	
	if(!tick) {
		packet.type = PROTO_TYPE_CONTROL;
		network_send(config.server.addr, &packet, sizeof(struct proto_control_packet));
	}
	
	tick++;
	tick %= 3;
}
