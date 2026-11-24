#include "tui.h"
#include "events.h"
#include "constants.h"

#include "window_start.h"
#include "window_levels.h"
#include "window_scoreboard.h"
#include "window_game.h"
#include "window_win.h"

#include "window_context.h"

typedef struct{
  char player_name[MAX_PLAYER_NAME_LENGTH + 1];
  char level_name[MAX_LEVEL_NAME_LENGTH + 1];
} WindowContextState;

void on_finished_start(TuiObject *self, TUI_Event event){
  WindowContextState *state = (WindowContextState *)self->state;
  strncpy(state->player_name, event.finsihed_start.player_name, MAX_PLAYER_NAME_LENGTH);
  
  tui_remove_and_free_all_child(self);
  w_levels_init(self, self->p_event_system);
}

void on_go_to_level_select(TuiObject *self, TUI_Event event){
  (void)event; //shuts up the compiler about unused parameter warning
  tui_remove_and_free_all_child(self);
  w_levels_init(self, self->p_event_system);
}

void on_go_to_scoreboard(TuiObject *self, TUI_Event event){
  WindowContextState *state = (WindowContextState *)self->state;
  if(event.scoreboard.level_name != NULL)
    strncpy(state->level_name, event.scoreboard.level_name, MAX_LEVEL_NAME_LENGTH);

  tui_remove_and_free_all_child(self);
  w_scoreboard_init(self, state->level_name, self->p_event_system);
}

void on_start_game(TuiObject *self, TUI_Event event){
  WindowContextState *state = (WindowContextState *)self->state;
  if(event.scoreboard.level_name != NULL)
    strncpy(state->level_name, event.scoreboard.level_name, MAX_LEVEL_NAME_LENGTH);

  tui_remove_and_free_all_child(self);
  w_game_init(self, state->level_name, state->player_name, self->p_event_system);
}

void on_won_game(TuiObject *self, TUI_Event event){
  WindowContextState *state = (WindowContextState *)self->state;

  tui_remove_and_free_all_child(self);
  w_win_init(self, self->p_event_system, state->player_name, state->level_name, event.won_game.move_count);
}

void window_context_on_event(TuiObject *self, TUI_Event event){
  switch (event.type) {
  case EVENT_FINISHED_START:
    on_finished_start(self, event);
    break;
  case EVENT_GOTOLEVELSELECET:
    on_go_to_level_select(self, event);
    break;
  case EVENT_GOTOSCOREBOARD:
    on_go_to_scoreboard(self, event);
    break;
  case EVENT_STARTGAME:
    on_start_game(self, event);
    break;
  case EVENT_WONGAME:
    on_won_game(self, event);
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
  tui_tuiobj_add_state(window_context_root, sizeof(WindowContextState), NULL);

  w_start_init(window_context_root, event_system);
}