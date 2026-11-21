#include <econio/econio.h>
#include <vec2/vec2.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>

#include "debugmalloc.h"

#include "game.h"
#include "tui.h"
#include "window_start.h"


void start_tui(){
    TuiObject *root = tui_create_tuiobj(NULL, &(TuiObjectDimensions){
        .horizontal_alignment = ALIGN_START,
        .vertical_alignment = ALIGN_START,
        .offset = i32vec2_create(0,0),
        .size = i32vec2_create(80,30),
    }, 
    NULL);

    TuiObject *a1 = tui_create_tuiobj(root, &(TuiObjectDimensions){
        .vertical_alignment = ALIGN_CENTER,
        .horizontal_alignment = ALIGN_CENTER,
        .offset = i32vec2_create(10,0),
        .size = i32vec2_create(50,10)
    },
    tui_renderaction_setbgtowhite);

    TuiObject *a2 = tui_create_tuiobj(root,NULL,NULL);
    TuiObject *a3 = tui_create_tuiobj(root,NULL,NULL);

    TuiObject *b1 = tui_create_tuiobj(a1, &(TuiObjectDimensions){
        .vertical_alignment = ALIGN_END,
        .horizontal_alignment = ALIGN_END,
        .offset = i32vec2_create(0,0),
        .size = i32vec2_create(5,5)
    },
    tui_renderaction_DEFAULT_SETTINGS);
    TuiObject *b2 = tui_create_tuiobj(a1, &(TuiObjectDimensions){
        .vertical_alignment = ALIGN_END,
        .horizontal_alignment = ALIGN_CENTER,
        .offset = i32vec2_create(0,0),
        .size = i32vec2_create(5,6)
    },
    tui_renderaction_DEFAULT_SETTINGS);
    TuiObject *b3 = tui_create_tuiobj(a1,NULL,NULL);

    tui_set_content(a1, "asdfasd");
    tui_set_content(a1, "asdklj");

    //tui_debug_tree(root, 0);
    tui_render(root);

    tui_free_tuiobj(a1);
    //tui_debug_tree(root, 0);
    tui_free_tuiobj(root);
}

void main_game_loop(){
    bool exit = false;

    TuiObject *root = tui_create_tuiobj(NULL, &(TuiObjectDimensions){
        .horizontal_alignment = ALIGN_START,
        .vertical_alignment = ALIGN_START,
        .offset = i32vec2_create(0,0),
        .size = i32vec2_create(80,30),
    }, 
    NULL);

    w_start_init(root);

    while (!exit)
    {
        tui_render(root);
        econio_rawmode();
        switch(econio_getch()){
            case KEY_UP:
                break;
            case KEY_DOWN:
                break;
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case KEY_ENTER:
                break;
            case KEY_ESCAPE:
                exit = true;
            break;
        }
    }
    tui_free_tuiobj(root);
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