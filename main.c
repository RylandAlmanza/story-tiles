#include "interface.h"
#include "scene.h"
#include "start_scene.h"
#include "play_scene.h"
#include "strings.h"

int main() {
    load_strings();
    interface_init();
    game_over = false;
    start_start_scene();
    update_scene = &update_start_scene;
    while (game_over == false) {
	int key = getch();
	update_scene(key);
    }
    interface_uninit();
    printf("Thanks for playing!\n");
}
