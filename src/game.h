#ifndef GAME_H
#define GAME_H

#include <vec2/vec2.h>

typedef struct {
    Vec2 player;
    Vec2 size;
    char **map;
    int wallcount;
    int boxcount;
} State;

// from: http://sokoban.org/about_sokoban.php 
typedef enum {
    CT_MAN = '@',
    CT_MAN_ON_GOAL = '+',
    CT_BOX = '$',
    CT_BOX_ON_GOAL = '*',
    CT_WALL = '#',
    CT_GOAL = '.',
    CT_FLOOR  = ' ',
} CellType;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

void game_end(State *state);
void game_start(State *game_state);
void game_init(State *game_state);

#endif