#ifndef GAME_H
#define GAME_H

#include <vec2/vec2.h>

typedef struct {
    i32Vec2 player;
    i32Vec2 size;
    char **map;
    int wallcount;
    int boxcount;
} Game_State;

// from: http://sokoban.org/about_sokoban.php 
typedef enum {
    CT_MAN = '@',
    CT_MAN_ON_GOAL = '+',
    CT_BOX = '$',
    CT_BOX_ON_GOAL = '*',
    CT_WALL = '#',
    CT_GOAL = '.',
    CT_FLOOR  = ' ',
} Game_CellType;

typedef enum {
    DIR_UP = 0b00,
    DIR_DOWN = 0b11,
    DIR_LEFT = 0b01,
    DIR_RIGHT = 0b10
} Game_Direction;

void game_start(Game_State *game_state);
void game_init(Game_State *game_state);
void game_free_game_state(Game_State *state);
Game_State *game_setup_game_state(char *level_name, Game_State *state);

#endif