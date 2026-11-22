#include <econio/econio.h>
#include <vec2/vec2.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>

#include "debugmalloc.h"

#include "game.h"
#include "tui.h"
#include "window_start.h"
#include "window_levels.h"

void main_game_loop(){
    bool exit = false;

    Screen *screen = tui_create_screen(i32vec2_create(80,30));

    TuiObject *root = tui_create_tuiobj(NULL, &(TuiObjectDimensions){
        .horizontal_alignment = ALIGN_START,
        .vertical_alignment = ALIGN_START,
        .offset = i32vec2_create(0,0),
        .size = i32vec2_create(screen->size.x, screen->size.y),
    });

    w_levels_init(root);
    //w_start_init(root);

    econio_rawmode();
    EconioKey key;
    while (!exit)
    {
        tui_render(root, screen);
        switch(key = econio_getch()){
            case KEY_ESCAPE:
                exit = true;
            break;
            default:
                tui_dispatch_event(root, (TUI_Event){
                    .keypress.type = EVENT_KEYPRESS,
                    .keypress.key = key 
                });
            break;
        }
    }

    tui_free_tuiobj(root);
    tui_free_screen(screen);
}


int main(void) {
    setlocale(LC_ALL, ".UTF8");

    main_game_loop();

    return 0;
    //start_menu();
    
    State game_state;
    game_init(&game_state);
    game_start(&game_state);
    game_end(&game_state);
    return 0;
}