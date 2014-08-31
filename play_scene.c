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

void ground_call() {
    char text[256] = "You are standing on a weathered, gray stone floor.";
    for (int i = 0; i < strlen(text); i++) {
	interface_draw(i, world_height, text[i], WHITE, BLACK);
    }
}

void wall_call() {
    char text[256] = "These walls look very old.";
    for (int i = 0; i < strlen(text); i++) {
	interface_draw(i, world_height, text[i], WHITE, BLACK);
    }
}

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
	    world.mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE |
		               COMPONENT_INTERACTION | COMPONENT_SPACE;
	    world.position[tile].x = i;
	    world.position[tile].y = world_height;
	    world.appearance[tile].chr = line[i];
	    world.appearance[tile].fg = WHITE;
	    world.appearance[tile].bg = BLACK;
	    world.interaction[tile].call = line[i] == '#' ? &wall_call :
		                                            &ground_call;
	    world.space[tile].occupant = line[i] == '#' ? tile : OCCUPANT_NONE;
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
    world.space[(1 * world_width) + 1].occupant = player;
    fclose(map_file);
}

// This function returns -1 if it successfully moves the entity; Otherwise, it
// returns the id of the entity that blocked movement (e.g. a wall tile's id)

int move_entity_by(int entity, int x_delta, int y_delta) {
    int old_x = world.position[entity].x;
    int old_y = world.position[entity].y;
    int new_x = old_x + x_delta;
    int new_y = old_y + y_delta;
    int old_tile = (old_y * world_width) + old_x;
    int new_tile = (new_y * world_width) + new_x;
    if (old_tile == new_tile) {
	return -1;
    }
    if (world.space[new_tile].occupant >= 0) {
	return world.space[new_tile].occupant;
    }
    world.space[old_tile].occupant = -1;
    world.space[new_tile].occupant = entity;
    world.position[entity].x = new_x;
    world.position[entity].y = new_y;
    return -1;
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
    interface_clear_line(world_height);
    if (key == 'q') {
	game_over = true;
	return;
    }
    int x_delta = 0;
    int y_delta = 0;
    if (key == KEY_UP) {
	y_delta = -1;
    }
    if (key == KEY_RIGHT) {
	x_delta = 1;
    }
    if (key == KEY_DOWN) {
        y_delta = 1;
    }
    if (key == KEY_LEFT) {
	x_delta = -1;
    }
    int blocking_entity = move_entity_by(player, x_delta, y_delta);
    if (blocking_entity >= 0) {
	world.interaction[blocking_entity].call();
    }
    if (key == 'x') {
	int tile_x = world.position[player].x;
	int tile_y = world.position[player].y;
	int tile = (tile_y * world_width) + tile_x;
	world.interaction[tile].call();
    }
    for (int i = 0; i < MAX_ENTITIES; i++) {
	draw_entity(i);
    }
}
