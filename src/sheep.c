#include "farmer.h"
#include "sheep.h"
#include "limits.h"

struct sheep_gravity {
	int			dx;
	int			dy;
};


void sheep_loop() {
	#if 0
	int i, j;

	/* Counting sheep... Zzz... */
	for (i = 0; i < SHEEP_COUNT; i++) {
		for (j = 0; j < FARMER_COUNT; i++) {
			dx = server_state.sheep[i].x - server_state.plist[j].x;
			dy = server_state.sheep[i].x - server_state.plist[j].y;
			if (dx * dx + dy * dy < dist_c) {
				farmer_c = j;
				dist_c = dx * dx + dy * dy;
			}
		}
		
		/* When we know what farmer to run from */
		/* TODO: Make sheep follow other sheep */
	}
	#endif
	return;
}
