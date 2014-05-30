#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <darnit/darnit.h>

void menu_loop() {
	DARNIT_KEYS keys;
	keys = d_keys_get();
	d_keys_set(keys);
	
	if(keys.select) {
		d_quit();
	}
}
