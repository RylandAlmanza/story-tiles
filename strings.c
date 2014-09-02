#include <stdio.h>
#include "strings.h"

void load_strings() {
    FILE *string_file = fopen("strings.txt", "r");
    fgets(START_TEXT, 256, string_file);
    fgets(FLOOR_DESCRIPTION, 256, string_file);
    fgets(WALL_DESCRIPTION, 256, string_file);
    fgets(ARROW_DESCRIPTION, 256, string_file);
    fgets(DOOR_DESCRIPTION, 256, string_file);
}
