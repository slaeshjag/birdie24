#ifndef __SERVER_H__
#define	__SERVER_H__

#define	SERVER_PORT			34543
#define	SERVER_FARMER_SLEEPING		500


#include "farmer.h"
#include "proto.h"
#include "network.h"
#include "sheep.h"

struct server_player_list_e {
	int				used;
	char				player_name[PROTO_PLAYER_NAME];
	/* Does not work for IPv6. But it's less than 24 hours to deadline #YOLO */
	unsigned long			addr;
	int				timeout;

	int				x;
	int				y;
	int				dx;
	int				dy;

	int				stab_timeout;

	/* Key status */
	unsigned int			left;
	unsigned int			right;
	unsigned int			up;
	unsigned int			down;
};


struct server_player_state {
	int				enabled;
	struct server_player_list_e	plist[FARMER_COUNT];
	struct Sheep			sheep[SHEEP_COUNT];
	struct proto_packet		pp;
	char				name[PROTO_GAME_NAME_MAX];
	char				map[PROTO_MAP_NAME_MAX];

	unsigned int			sheep_collide[SHEEP_COUNT];
} server_state;

void server_init();
void server_start(const char *game_name);
void server_loop();
void server_handle_packet(void *packet, unsigned long addr);

#endif
