#ifndef __SHEEP_H__
#define	__SHEEP_H__

#define	SHEEP_COUNT		99
#define	SHEEP_AVOID_RADIUS	140

struct Sheep {
	int			x;
	int			y;
	int			panic;
	int			pg_x;
	int			pg_y;
	int			dx;
	int			dy;
	int			angle;
};

void sheep_prepare();
void sheep_init();

void sheep_spawn();
void sheep_loop();

#endif
