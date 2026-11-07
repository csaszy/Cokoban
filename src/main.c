#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <vec2/vec2.h>
#include <tui/tui.h>
#include <econio/econio.h>
#include "debugmalloc.h"

typedef struct {
    Vec2 player;
    Vec2 size;
    char **map;
    int wallcount;
} State;

// from: http://sokoban.org/about_sokoban.php 
typedef enum {
    MAN = '@',
    MAN_ON_GOAL = '+',
    BOX = '$',
    BOX_ON_GOAL = '*',
    WALL = '#',
    GOAL = '.',
    FLOOR  = ' ',
} CellType;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

    
int load_level(char *name, State *state){
    char path[256];
    sprintf(path, "levels/%s.xsb", name);

    FILE *file = fopen(".\\assets\\levels\\level001.xsb", "r");
    if(file == NULL){
        perror("Level nem található");
        return 1;
    }

    state->size.x = 0;
    state->size.y = 0;
    state->wallcount = 0;

    int char_count = 0;
    char c;
    while((c = fgetc(file)) != EOF){
        if(c == '\n'){
            state->size.y++;
            char_count = 0;
        }else{
            state->size.x = fmax(++char_count, state->size.x);
        }
    }

    state->map = (char**) malloc(state->size.y * sizeof(char*));
    state->map[0] = (char*) malloc(state->size.y * state->size.x * sizeof(char));
    for(int i = 1; i < state->size.y; i++){
        state->map[i] = state->map[0] + i * (int)state->size.x;
    }

    fseek(file, 0, SEEK_SET);

    for(int y = 0; y < state->size.y; ++y){
        for(int x = 0; x < state->size.x; ++x){
            c = fgetc(file);
            switch(c){
                case '\n':
                    ungetc('\n', file);
                    state->map[y][x] = FLOOR;
                    break;
                case WALL:
                    state->wallcount++;
                    state->map[y][x] = c;
                    break;
                case MAN:
                    state->player = (Vec2){x, y};
                    state->map[y][x] = c;
                    break;
                default:
                    state->map[y][x] = c;
            }
        }
        fgetc(file);
    }

    fclose(file);
    return 0;
}

void game_print(State *s){
    econio_clrscr();
    char c;
    for(int y = 0; y < s->size.y; ++y){
        for(int x = 0; x < s->size.x; ++x){
            printf("%c", s->map[y][x]);
        }
        printf("\n");
    }
}

int virtual_move_player(Direction dir, State *dst, State *src){
    #define DPX (int)dst->player.x //I'll fix Vec2 library later 
    #define DPY (int)dst->player.y //till then this will do
    #define SPX (int)src->player.x //I'll fix Vec2 library later 
    #define SPY (int)src->player.y //till then this will do
    switch (dir){
        case DIR_UP:
            dst->map[DPY][DPX] = ' ';
            dst->player.y = fmax(dst->player.y - 1, 0); 
            dst->map[DPY][DPX] = '@';
            break;
        case DIR_DOWN:
            dst->map[DPY][DPX] = ' ';
            dst->player.y = fmin(dst->player.y + 1, dst->size.y); 
            dst->map[DPY][DPX] = '@';
            break;
        case DIR_LEFT:
            dst->map[DPY][DPX] = ' ';
            dst->player.x = fmax(dst->player.x - 1, 0); 
            dst->map[DPY][DPX] = '@';
            break;
        case DIR_RIGHT:
            dst->map[DPY][DPX] = ' ';
            dst->player.x = fmin(dst->player.x + 1, dst->size.x); 
            dst->map[DPY][DPX] = '@';
            break;
        
        default:
            break;
    }
}

int count_walls(State *state){
    int count = 0;
    for(int y = 0; y < state->size.y; ++y){
        for(int x = 0; x < state->size.x; ++x){
            if(state->map[y][x] == WALL) ++count;
        }
    }
    return count;
}

State create_copy_state(State *src){
    State dst;
    dst.player = src->player;
    dst.size = src->size;
    dst.wallcount = src->wallcount;

    dst.map = (char**) malloc(dst.size.y * sizeof(char*));
    dst.map[0] = (char*) malloc(dst.size.y * dst.size.x * sizeof(char));
    for(int i = 1; i < dst.size.y; i++){
        dst.map[i] = dst.map[0] + i * (int)dst.size.x;
    }

    memcpy(dst.map[0], src->map[0], dst.size.y * dst.size.x * sizeof(char));
    return dst;
}

void copy_state(State *dst, State *src){
    dst->player = src->player;
    dst->size = src->size;
    dst->wallcount = src->wallcount;
    memcpy(dst->map[0], src->map[0], dst->size.y * dst->size.x * sizeof(char));
}

void free_state(State *state){
    free(state->map[0]);
    free(state->map);
}

/*
    Mozgatja a játékost a megadott irányba, ha lehetséges.
    Visszatérési érték: 0 -> sikeres mozgás, 1 -> sikertelen mozgás
*/
int move_player(Direction dir, State *state){
    State new_state = create_copy_state(state);

    virtual_move_player(dir, &new_state);
    if(state->wallcount != count_walls(&new_state)){ //not super efficient collision check
        free_state(&new_state);
        return 1; 
    } 

    copy_state(state, &new_state);    
    free_state(&new_state);
     
    return 0;
}

void game_end(State *state){
    free(state->map[0]);
    free(state->map);
}

void game_start(){
    State game_state;
    load_level("level001", &game_state);

    bool exit = false;
    econio_rawmode();
    while (!exit)
    {
        game_print(&game_state);

        switch(econio_getch()){
            case KEY_UP:
                move_player(DIR_UP, &game_state);
                break;
            case KEY_DOWN:
                move_player(DIR_DOWN, &game_state);
                break;
            case KEY_LEFT:
                move_player(DIR_LEFT, &game_state);
                break;
            case KEY_RIGHT:
                move_player(DIR_RIGHT, &game_state);
                break;
            case KEY_ESCAPE:
                exit = true;
            break;
        }
    }

    game_end(&game_state);
}
 
int main(int argc, char *argv[]) {
    game_start();
    return 0;
}