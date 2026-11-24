#include <string.h>
#include <math.h>
#include <assert.h>

#include "debugmalloc.h" 
#include <vec2/vec2.h>
#include <econio/econio.h> 

#include "constants.h"
#include "game.h"

void game_print(Game_State *s){
    econio_rawmode();
    econio_gotoxy(0, 0);
    for(int y = 0; y < s->size.y; ++y){
        for(int x = 0; x < s->size.x; ++x){
            printf(" %c", s->map[y][x]);
        }
        printf("\n");
    }
}

void virtual_move_player(Game_Direction dir, Game_State *dst){
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

int count_celltype(Game_State *state, Game_CellType ct){
    int count = 0;
    for(int y = 0; y < state->size.y; ++y){
        for(int x = 0; x < state->size.x; ++x){
            if(state->map[y][x] == ct) ++count;
        }
    }
    return count;
}

Game_State create_copy_state(Game_State *src){
    Game_State dst;
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

void copy_state(Game_State *dst, Game_State *src){
    dst->player = src->player;
    dst->size = src->size;
    dst->wallcount = src->wallcount;
    memcpy(dst->map[0], src->map[0], dst->size.y * dst->size.x * sizeof(char));
}

void resolve_box_collision(Game_Direction dir, Game_State *state){
    i32Vec2 target = state->player;
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

void free_state(Game_State *state){
    free(state->map[0]);
    free(state->map);
}

int game_move_player(Game_Direction dir, Game_State *state){
    Game_State new_state = create_copy_state(state);

    virtual_move_player(dir, &new_state);
    if(state->boxcount != count_celltype(&new_state, CT_BOX) + count_celltype(&new_state, CT_BOX_ON_GOAL)){
        resolve_box_collision(dir, &new_state);
    } 

    if(state->wallcount != count_celltype(&new_state, CT_WALL)){ 
        free_state(&new_state);
        return 1; 
    } 

    copy_state(state, &new_state);    
    free_state(&new_state);

    state->move_count++;
     
    return 0;
}

void game_free_game_state_internals(void *game_state){
    Game_State *state = (Game_State *)game_state; 
    free(state->map[0]);
    free(state->map);
}

Game_State *game_setup_game_state(Game_State *state, char *level_name, char *player_name){
    if(state == NULL) return NULL;

    state->size = i32vec2_create(0, 0);
    state->wallcount = 0;
    state->boxcount = 0;
    state->p_player_name = player_name;
    state->p_level_name = level_name;
    state->move_count = 0;

    char path[MAX_LEVEL_NAME_LENGTH + 50]; //yeah ik
    sprintf(path, ".\\assets\\levels\\%s.xsb", level_name);

    FILE *file = fopen(path, "r");
    if(file == NULL){
        perror("Level nem található");
        return NULL;
    }

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
                    state->player = i32vec2_create(x, y);
                    state->map[y][x] = c;
                    break;
                case CT_MAN_ON_GOAL:
                    state->player = i32vec2_create(x, y);
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
    return state;
}