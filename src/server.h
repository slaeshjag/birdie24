#ifndef __SERVER_H__
#define	__SERVER_H__

#define	SERVER_PORT			34543

#include "proto.h"

struct server_player_list_e {
	int				used;
	char				player_name[16];
	/* TODO: Add structure for UDP IP */
};


struct server_player_state {
	struct server_plater_list_e	plist[PLAYER_MAX];
	struct proto_packet		pp;
};

#endif
