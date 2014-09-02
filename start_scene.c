#include <string.h>
#include <stdbool.h>
#include "scene.h"
#include "start_scene.h"
#include "play_scene.h"
#include "interface.h"
#include "strings.h"

void start_start_scene() {
    for (int i = 0; i < strlen(START_TEXT); i++) {
	interface_draw(i, 0, START_TEXT[i], WHITE, BLACK);
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
