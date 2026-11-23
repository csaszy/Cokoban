#include "tui.h"
#include "events.h"
#include "window_start.h"
#include "window_levels.h"
#include "window_game.h"

typedef struct{
    EventSystem *p_event_system;
} WindowContextState;

void on_start_game(TuiObject *self, TUI_Event event){
    //there really should only be one children, de "biztos ami tuti"
    for(int i = 0; i < darr_size(self->children); ++i){
        TuiObject **child = (TuiObject **)darr_get(self->children, i);
        tui_free_tuiobj(*child);
    }

    w_game_init(self, event.start_game.level_name, self->p_event_system);
}

void window_context_on_event(TuiObject *self, TUI_Event event){
    switch (event.type) {
    case EVENT_STARTGAME:
        on_start_game(self, event);
        break;
    default:
        break;
    }
}

void w_context_init(TuiObject *root, EventSystem *event_system){
  TuiObject *window_context_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = root->size 
  });
  tui_events_subscribe(window_context_root, window_context_on_event, event_system);

  w_levels_init(window_context_root, event_system);
}