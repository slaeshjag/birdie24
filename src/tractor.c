#if 0
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <darnit/darnit.h>

static struct KeyFrame {
	int x;
	int y;
	int delta;
};

static struct KeyFrame keyframe[] = {
	{400, 600, 0},
	{780, 300, 60},
	{20, 300, 60},
	{400, 0, 60},
	{-1, -1, -1},
};

static int length(int x1, int y1, int x2, int y2) {
	int w = abs(x2 - x1);
	int h = abs(y2 - y1);
	return sqrt(w*w + h*h)
}

static int angle(int x1, int y1, int x2, int y2) {
	double dx1 = x1, dy1 = y1, dx2 = x2, dy2 = y2;
	return 10*180*atan((dy2 - dy1)/(dx2 - dx1))/M_PI;
}

void tractor_move() {
	struct KeyFrame *kf = &keyframe[server_state.tractor.keyframe];
	if(server_state.tractor.delta >= kf->delta) {
		struct KeyFrame *kf_new = &keyframe[server_state.tractor.keyframe + 1];
		if(kf_new->delta < 0) {
			//tractor done
			return;
		}
		
		server_state.tractor.speed = length(server_state.tractor.x, server_state.tractor.y, kf->x, kf->y)*10/(kf->delta*10);
		server_state.tractor.angle = angle(server_state.tractor.x, server_state.tractor.y, kf->x, kf->y);
	}
	server_state.tractor.x = 0;
	
}

#endif
