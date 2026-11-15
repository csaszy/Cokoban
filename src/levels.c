#include <stdio.h>
#include <math.h>

#include <vec2/vec2.h>
#include "debugmalloc.h"
#include "game.h"
#include "levels.h"

int load_level(char *name, State *state){
    char path[256];
    sprintf(path, ".\\assets\\levels\\%s.xsb", name);

    FILE *file = fopen(path, "r");
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