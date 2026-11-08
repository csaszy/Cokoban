#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vec2/vec2.h>
#include <tui/tui.h>
#include <econio/econio.h>
#include "debugmalloc.h"

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
    state->boxcount = 0;

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
                    state->map[y][x] = CT_FLOOR;
                    break;
                case CT_WALL:
                    state->wallcount++;
                    state->map[y][x] = c;
                    break;
                case CT_MAN:
                    state->player = (Vec2){x, y};
                    state->map[y][x] = c;
                    break;
                case CT_MAN_ON_GOAL:
                    state->player = (Vec2){x, y};
                    state->map[y][x] = c;
                    break;
                case CT_BOX:
                    state->boxcount++;
                    state->map[y][x] = c;
                    break;
                case CT_BOX_ON_GOAL:
                    state->boxcount++;
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
    econio_gotoxy(0, 0);
    char c;
    for(int y = 0; y < s->size.y; ++y){
        for(int x = 0; x < s->size.x; ++x){
            printf("%c", s->map[y][x]);
        }
        printf("\n");
    }
}

int virtual_move_player(Direction dir, State *dst){
    #undef DST_PLAYER_CELL
    #define DST_PLAYER_CELL dst->map[(int)dst->player.y][(int)dst->player.x]
    switch (dir){
        case DIR_UP:
            DST_PLAYER_CELL = DST_PLAYER_CELL == CT_MAN_ON_GOAL ? CT_GOAL : CT_FLOOR;
            dst->player.y = fmax(dst->player.y - 1, 0); 
            if(DST_PLAYER_CELL == CT_GOAL || DST_PLAYER_CELL == CT_BOX_ON_GOAL){
                DST_PLAYER_CELL = CT_MAN_ON_GOAL;
            }else{
                DST_PLAYER_CELL = CT_MAN;
            }
            break;
        case DIR_DOWN:
            DST_PLAYER_CELL = DST_PLAYER_CELL == CT_MAN_ON_GOAL ? CT_GOAL : CT_FLOOR;
            dst->player.y = fmin(dst->player.y + 1, dst->size.y); 
            if(DST_PLAYER_CELL == CT_GOAL || DST_PLAYER_CELL == CT_BOX_ON_GOAL){
                DST_PLAYER_CELL = CT_MAN_ON_GOAL;
            }else{
                DST_PLAYER_CELL = CT_MAN;
            }
            break;
        case DIR_LEFT:
            DST_PLAYER_CELL = DST_PLAYER_CELL == CT_MAN_ON_GOAL ? CT_GOAL : CT_FLOOR;
            dst->player.x = fmax(dst->player.x - 1, 0); 
            if(DST_PLAYER_CELL == CT_GOAL || DST_PLAYER_CELL == CT_BOX_ON_GOAL){
                DST_PLAYER_CELL = CT_MAN_ON_GOAL;
            }else{
                DST_PLAYER_CELL = CT_MAN;
            }
            break;
        case DIR_RIGHT:
            DST_PLAYER_CELL = DST_PLAYER_CELL == CT_MAN_ON_GOAL ? CT_GOAL : CT_FLOOR;
            dst->player.x = fmin(dst->player.x + 1, dst->size.x); 
            if(DST_PLAYER_CELL == CT_GOAL || DST_PLAYER_CELL == CT_BOX_ON_GOAL){
                DST_PLAYER_CELL = CT_MAN_ON_GOAL;
            }else{
                DST_PLAYER_CELL = CT_MAN;
            }
            break;
        
        default:
            break;
    }
    #undef DST_PLAYER_CELL
}

int count_celltype(State *state, CellType ct){
    int count = 0;
    for(int y = 0; y < state->size.y; ++y){
        for(int x = 0; x < state->size.x; ++x){
            if(state->map[y][x] == ct) ++count;
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

void resolve_box_collision(Direction dir, State *state){
    Vec2 target = state->player;
    #undef TARGET_CELL
    #define TARGET_CELL state->map[(int)target.y][(int)target.x]
    
    do {
        switch (dir) { 
            case DIR_UP:    --target.y; break;
            case DIR_DOWN:  ++target.y; break;
            case DIR_LEFT:  --target.x; break;
            case DIR_RIGHT: ++target.x; break;
        }
    } while (TARGET_CELL == CT_BOX || TARGET_CELL == CT_BOX_ON_GOAL);
   
    TARGET_CELL = TARGET_CELL == CT_GOAL ? CT_BOX_ON_GOAL : CT_BOX;
    #undef TARGET_CELL
}

/*
    Mozgatja a játékost a megadott irányba, ha lehetséges.
    Visszatérési érték: 0 -> sikeres mozgás, 1 -> sikertelen mozgás
*/
int move_player(Direction dir, State *state){
    State new_state = create_copy_state(state);

    virtual_move_player(dir, &new_state);
    if(state->boxcount != count_celltype(&new_state, CT_BOX) + count_celltype(&new_state, CT_BOX_ON_GOAL)){
        resolve_box_collision(dir, &new_state);
    } 

    //not the most efficient thing in the world, but at least it's readable (it ain't) 
    if(state->wallcount != count_celltype(&new_state, CT_WALL)){ 
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

    econio_rawmode();
    econio_clrscr();

    bool exit = false;
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

        //just to be safe
        assert(count_celltype(&game_state, CT_BOX) + count_celltype(&game_state, CT_BOX_ON_GOAL) == game_state.boxcount); 
        assert(count_celltype(&game_state, CT_WALL) == game_state.wallcount); 
        assert(count_celltype(&game_state, CT_BOX) == count_celltype(&game_state, CT_GOAL) + count_celltype(&game_state, CT_MAN_ON_GOAL)); 
    }

    game_end(&game_state);
}
 
int main(int argc, char *argv[]) {
    game_start();
    return 0;
}