#include <string.h>
#include <stdbool.h>
#include "scene.h"
#include "start_scene.h"
#include "play_scene.h"
#include "interface.h"

void start_start_scene() {
    char start_text[256] = "Press 'x' to start or 'q' to quit";
    for (int i = 0; i < strlen(start_text); i++) {
	interface_draw(i, 0, start_text[i], WHITE, BLACK);
	interface_flush();
    }
}

void update_start_scene(int key) {
    if (key == 'q') {
	game_over = true;
    }
    if (key == 'x') {
	start_play_scene();
	update_scene = &update_play_scene;
	return;
    }
}
