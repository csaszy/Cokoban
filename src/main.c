#include <econio/econio.h>
#include <vec2/vec2.h>
#include <locale.h>
#include <windows.h>
#include <stddef.h>

#include "debugmalloc.h"

#include "tui_windows/window_context.h"
#include <tui.h>

void main_game_loop(){
    const i32Vec2 default_screen_size = {80, 30};
    i32Vec2 terminal_window_size = {0};

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    terminal_window_size.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    terminal_window_size.y = csbi.srWindow.Bottom - csbi.srWindow.Top;

    Screen *screen = NULL;
    if(terminal_window_size.x > 10 && terminal_window_size.y > 10){
        screen = tui_create_screen(terminal_window_size);
    }else{
        screen = tui_create_screen(default_screen_size);
    }

    TuiObject *root = tui_create_tuiobj(NULL, &(TuiObjectDimensions){
        .horizontal_alignment = ALIGN_START,
        .vertical_alignment = ALIGN_START,
        .offset = i32vec2_create(0,0),
        .size = i32vec2_create(screen->size.x, screen->size.y),
    });

    EventSystem *event_system = tui_create_event_system();
    w_context_init(root, event_system);

    econio_rawmode();
    bool exit = false;
    int key; //can't be EconioKey because the compiler get's mad
    while (!exit)
    {
        tui_render(root, screen);
        switch(key = econio_getch()){
            case 'q':
                exit = true;
            break;
            default:
                tui_events_push((TUI_Event){
                    .keypress.type = EVENT_KEYPRESS,
                    .keypress.key = key 
                }, event_system);
            break;
        }
        tui_dispatch_events_in_queue(root, event_system);
    }

    tui_free_tuiobj(root);
    tui_free_screen(screen);
    tui_free_event_system(event_system);
}


int main(void) {
    setlocale(LC_ALL, ".UTF8");

    main_game_loop();

    return 0;
}