#ifndef __SHEEP_H__
#define	__SHEEP_H__

#define	SHEEP_COUNT		299
#define	SHEEP_AVOID_RADIUS	150

struct sheep {
	int			x;
	int			y;
	int			panic;
	int			pg_x;
	int			pg_y;
};

void sheep_spawn();
void sheep_loop();

#endif
