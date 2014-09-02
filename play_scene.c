#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "scene.h"
#include "interface.h"
#include "world.h"
#include "play_scene.h"
#include "strings.h"

World world;
int world_width;
int world_height;
int player;

void do_nothing(int self, int other) {};

void ground_call(int self, int other) {
    if (other != player) {
	return;
    }
    interface_write(0, world_height, FLOOR_DESCRIPTION);
}

void wall_call(int self, int other) {
    if (other != player) {
	return;
    }
    interface_write(0, world_height, WALL_DESCRIPTION);
}

void arrow_call(int self, int other) {
    if (other != player) {
	return;
    }
    interface_write(0, world_height, ARROW_DESCRIPTION);
}

void door_call(int self, int other) {
    if (other != player) {
	return;
    }
    interface_write(0, world_height, DOOR_DESCRIPTION);
}

void door_enter(int self, int other) {
    if (other != player) {
	return;
    }
    interface_write(0, world_height, DOOR_ENTER_NOTIFICATION);
}

void door_exit(int self, int other) {
    if (other != player) {
	return;
    }
    interface_write(0, world_height, DOOR_EXIT_NOTIFICATION);
}

void load_map() {
    FILE *map_file = fopen("map.txt", "r");
    char line[256];
    world_width = 0;
    world_height = 0;
    int player_x, player_y;
    while (fscanf(map_file, "%s", line) != EOF) {
	if (world_width == 0) {
	    world_width = strlen(line);
	}
	for (int i = 0; i < world_width; i++) {
	    int tile = create_entity(&world);
	    void (*on_enter)(int self, int other) = &do_nothing;
	    void (*call)(int self, int other) = &do_nothing;
	    void (*on_exit)(int self, int other) = &do_nothing;
	    int occupant = OCCUPANT_NONE;
	    world.mask[tile] = COMPONENT_POSITION | COMPONENT_APPEARANCE |
		               COMPONENT_INTERACTION | COMPONENT_SPACE;
	    world.position[tile].x = i;
	    world.position[tile].y = world_height;
	    world.appearance[tile].chr = line[i];
	    world.appearance[tile].fg = WHITE;
	    world.appearance[tile].bg = BLACK;
	    if (line[i] == '#') {
		call = &wall_call;
		occupant = tile;
	    }
	    if (line[i] == '.') {
		call = &ground_call;
	    }
	    if (line[i] == '|' || line[i] == '/' || line[i] == '\\' ||
		line[i] == '^') {
		call = &arrow_call;
	    }
	    if (line[i] == '+') {
		on_enter = &door_enter;
		call = &door_call;
		on_exit = &door_exit;
	    }
	    if (line[i] == '@') {
		player_x = i;
		player_y = world_height;
		call = &ground_call;
		world.appearance[tile].chr = '.';
	    }
	    world.interaction[tile].on_enter = on_enter;
	    world.interaction[tile].call = call;
	    world.interaction[tile].on_exit = on_exit;
	    world.space[tile].occupant = occupant;
	}
	world_height++;
    }
    player = create_entity(&world);
    world.mask[player] = COMPONENT_POSITION | COMPONENT_APPEARANCE;
    world.position[player].x = player_x;
    world.position[player].y = player_y;
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
    world.interaction[old_tile].on_exit(old_tile, entity);
    world.interaction[new_tile].on_enter(new_tile, entity);
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
	world.interaction[blocking_entity].call(blocking_entity, player);
    }
    if (key == 'x') {
	int tile_x = world.position[player].x;
	int tile_y = world.position[player].y;
	int tile = (tile_y * world_width) + tile_x;
	world.interaction[tile].call(tile, player);
    }
    for (int i = 0; i < MAX_ENTITIES; i++) {
	draw_entity(i);
    }
}
