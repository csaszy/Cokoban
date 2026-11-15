#include "game.h"
 
int main(int argc, char *argv[]) {
    State game_state;
    game_init(&game_state);
    game_start(&game_state);
    game_end(&game_state);
    return 0;
}