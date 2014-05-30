#ifndef __SHEEP_H__
#define	__SHEEP_H__

#define	SHEEP_COUNT		4
#define	SHEEP_AVOID_RADIUS	240
#define	SHEEP_PANIC_RADIUS	200
#define	SHEEP_PANIC_RANGE	4150

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
int sheep_test_collide(int x, int y, int dx, int dy, int t);
void sheep_panic(int x, int y);



#endif
