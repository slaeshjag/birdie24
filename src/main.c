#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <darnit/darnit.h>

int main(int argc, char **argv) {
	d_init_custom("birdie24", 800, 600, false, "birdie24", NULL);
	
	for(;;) {
		d_render_begin();
		d_render_end();
		d_loop();
	}
	
	d_quit();
	return 0;
}
