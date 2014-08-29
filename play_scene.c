#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "scene.h"
#include "interface.h"
#include "world.h"
#include "play_scene.h"

World world;
int world_width;
int world_height;
int player;

void load_map() {
    FILE *map_file = fopen("map.txt", "r");
    char line[256];
    world_width = 0;
    world_height = 0;
    while (fscanf(map_file, "%s", line) != EOF) {
	if (world_width == 0) {
	    world_width = strlen(line);
	}
	for (int i = 0; i < world_width; i++) {
	    int tile = create_entity(&world);
	    world.mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
	    world.position[tile].x = i;
	    world.position[tile].y = world_height;
	    world.appearance[tile].chr = line[i];
	    world.appearance[tile].fg = WHITE;
	    world.appearance[tile].bg = BLACK;
	}
	world_height += 1;
    }
    player = create_entity(&world);
    world.mask[player] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world.position[player].x = 1;
    world.position[player].y = 1;
    world.appearance[player].chr = '@';
    world.appearance[player].fg = WHITE;
    world.appearance[player].bg = BLACK;
    fclose(map_file);
}

#define DRAW_MASK (COMPONENT_POSITION | COMPONENT_APPEARANCE)
void draw_entity(int entity) {
    if ((world.mask[entity] & DRAW_MASK) == DRAW_MASK) {
	int x = world.position[entity].x;
	int y = world.position[entity].y;
	int chr = world.appearance[entity].chr;
	int fg = world.appearance[entity].fg;
	int bg = world.appearance[entity].bg;
	interface_draw(x, y, chr, fg, bg);
    }
}

void start_play_scene() {
    interface_wipe();
    load_map();
    for (int i = 0; i < MAX_ENTITIES; i++) {
	draw_entity(i);
    }
}

void update_play_scene(int key) {
    if (key == 'q') {
	game_over = true;
	return;
    }
    if (key == KEY_UP) {
	world.position[player].y -= 1;
    }
    if (key == KEY_RIGHT) {
	world.position[player].x += 1;
    }
    if (key == KEY_DOWN) {
	world.position[player].y += 1;
    }
    if (key == KEY_LEFT) {
	world.position[player].x -= 1;
    }
    for (int i = 0; i < MAX_ENTITIES; i++) {
	draw_entity(i);
    }
}
