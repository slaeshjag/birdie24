#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <darnit/darnit.h>

#include "game.h"
#include "server.h"

void game_loop() {
	server_loop();
}
