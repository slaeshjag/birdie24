#ifndef __SERVER_H__
#define	__SERVER_H__

#define	SERVER_PORT			34543

#include "proto.h"
#include "network.h"

struct server_player_list_e {
	int				used;
	char				player_name[16];
	/* Does not work for IPv6. But it's less than 24 hours to deadline #YOLO */
	unsigned long			addr;
};


struct server_player_state {
	struct server_plater_list_e	plist[PLAYER_MAX];
	struct proto_packet		pp;
} server_state;

#endif
