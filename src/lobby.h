#ifndef __LOBBY_H_
#define __LOBBY_H_

struct LobbyHost {
	const char *name;
	unsigned long addr;
};

struct LobbyClient {
	struct List *client;
	int clients;
} extern lobby_client;

void lobby_init();
void lobby_playername_loop();
void lobby_loop();
void lobby_host_loop();
void lobby_join_loop();

#endif
