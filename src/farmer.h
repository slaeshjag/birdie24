#ifndef __FARMER_H__
#define	__FARMER_H__

#include <stdbool.h>

#define	FARMER_COUNT		2
#define	FARMER_SPEED		240
#define	FARMER_INTRO_SPEED		3000

struct Farmer {
	int			x;
	int			y;
	bool			yell;
	bool			stab;
	bool			move;
	int			angle;
};

void farmer_spawn();
void farmer_move();
void farmer_intro();

#endif
