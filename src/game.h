#ifndef GAME_H
#define GAME_H

#include <vec2/vec2.h>
#include "constants.h"

typedef struct {
    i32Vec2 player;
    i32Vec2 size;
    char **map;
    int wallcount;
    int boxcount;
    char *p_player_name;
    char level_name[MAX_LEVEL_NAME_LENGTH+1];
    unsigned int move_count; 
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

int game_move_player(Game_Direction dir, Game_State *state);
Game_State *game_setup_game_state(Game_State *state, char *level_name, char *player_name);
void game_free_game_state_internals(void *game_state);

#endif