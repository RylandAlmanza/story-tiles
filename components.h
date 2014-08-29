#ifndef COMPONENTS_H
#define COMPONENTS_H

typedef enum ComponentEnum Component;

enum ComponentEnum {
    COMPONENT_NONE = 0,
    COMPONENT_POSITION = 1 << 0,
    COMPONENT_APPEARANCE = 1 << 2,
    COMPONENT_INTERACTION = 1 << 3
};

typedef struct AppearanceStruct Appearance;

struct AppearanceStruct {
    char chr;
    int fg;
    int bg;
};

typedef struct PositionStruct Position;

struct PositionStruct {
    int x;
    int y;
};

typedef struct InteractionStruct Interaction;

struct InteractionStruct {
    char text[1024];
};

#endif
