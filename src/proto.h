#ifndef __PROTO_H__
#define	__PROTO_H__

#include <stdio.h>
#include <stdint.h>
#include "sheep.h"
#include "farmer.h"

#define	PROTO_GAME_NAME_MAX		32
#define	PROTO_PLAYER_NAME		16
#define	PROTO_MAP_NAME_MAX		16

enum proto_packet_type {
	PROTO_TYPE_BROADCAST,
	PROTO_TYPE_CONTROL,
	PROTO_TYPE_JOIN,
	PROTO_TYPE_KEEPALIVE,
	PROTO_TYPE_LOBBY_STAT,
	PROTO_TYPE_GAMESTATE,
	PROTO_TYPE_JOIN_GREET,
	PROTO_TYPE_IDLE_BROADCAST,
	PROTO_TYPE_CHAT,
};

struct proto_coord {
	unsigned int			x	: 12;
	unsigned int			y	: 12;
	unsigned int			dir	: 4;
	unsigned int			moving	: 1;
} __attribute__((packed));


struct proto_farmer {
	struct proto_coord		coord;

	unsigned int			action_yell	: 1;
	unsigned int			action_stab	: 1;
	unsigned int			action_stabbed	: 1;
};

struct proto_packet {
	enum proto_packet_type		type;

	unsigned int			packet_cnt;
	unsigned int			time_remain;
	unsigned int			game_running;

	unsigned int			sheep_points[FARMER_COUNT];
	struct proto_coord		sheep[SHEEP_COUNT];
	struct proto_farmer		farmer[FARMER_COUNT];
	unsigned int			leader_sheep[FARMER_COUNT];
};


struct proto_control_packet {
	enum proto_packet_type		type;

	unsigned int			up	: 1;
	unsigned int			left	: 1;
	unsigned int			right	: 1;
	unsigned int			down	: 1;
	unsigned int			yell	: 1;
	unsigned int			stab	: 1;
};


struct proto_control_broadcast {
	enum proto_packet_type		type;
	char 				game_name[PROTO_GAME_NAME_MAX];
	char				map_name[PROTO_MAP_NAME_MAX];
	unsigned int			slots;
	unsigned int			slots_left;
};


struct proto_idling_broadcast {
	enum proto_packet_type		type;

	char				player_name[PROTO_PLAYER_NAME];
};


struct proto_chat {
	enum proto_packet_type		type;

	char				player_name[PROTO_PLAYER_NAME];
	int				broadcast;
};


struct proto_join_packet {
	enum proto_packet_type		type;

	char 				player_name[PROTO_PLAYER_NAME];
	int				player_id;
	uint32_t			addr;
};


struct proto_join_greet {
	enum proto_packet_type		type;

	unsigned int			player_num;
};


void proto_init();
void proto_send(struct proto_packet *pp);
void proto_recv(struct proto_packet *pp);

#endif
