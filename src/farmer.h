#ifndef __FARMER_H__
#define	__FARMER_H__

#include <stdbool.h>

#define	FARMER_COUNT		2
#define	FARMER_SPEED		240

struct Farmer {
	int			x;
	int			y;
	bool			yell;
	bool			stab;
	int			angle;
};

void farmer_spawn();
void farmer_move();

#endif
