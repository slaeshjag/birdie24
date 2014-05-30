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
		game.sheep.sprite[i] = d_sprite_new(config.tileset);
		d_sprite_frame_entry_set(game.sheep.sprite[i], 0, 0, 6, 200);
		d_sprite_activate(game.sheep.sprite[i], 0);
		d_sprite_move(game.sheep.sprite[i], 32, 32);
	}
	
	for(i = 0; i < FARMER_COUNT; i++) {
		game.farmer.sprite[i] = d_sprite_new(config.tileset);
		d_sprite_frame_entry_set(game.farmer.sprite[i], 0, 0, 0, 200);
		d_sprite_activate(game.farmer.sprite[i], 0);
		d_sprite_move(game.farmer.sprite[i], 32, 32);
	}
}

void game_loop() {
	static int tick = 0;
	int i;
	struct proto_control_packet packet;
	
	server_loop();
	for(i = 0; i < SHEEP_COUNT; i++) {
		d_sprite_move(game.sheep.sprite[i], game.sheep.sheep[i].x, game.sheep.sheep[i].y);
		d_sprite_draw(game.sheep.sprite[i]);
	}
	
	for(i = 0; i < FARMER_COUNT; i++) {
		d_sprite_move(game.farmer.sprite[i], game.farmer.farmer[i].x, game.farmer.farmer[i].y);
		d_sprite_draw(game.farmer.sprite[i]);
	}
	
	if(!tick) {
		packet.type = PROTO_TYPE_CONTROL;
		network_send(config.server.addr, &packet, sizeof(struct proto_control_packet));
	}
	
	tick++;
	tick %= 3;
}
