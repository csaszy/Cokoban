#include "tui.h"
#include <vec2/vec2.h>
#include <string.h>

#include "game.h"
#include "constants.h"
#include "window_constants.h"

#include "window_levels.h"

#define LEVEL_LIST_PADDING 3

typedef struct {
  TuiObject *selected_child;
  TuiObject *level_list;
} Levels_RootState;

typedef struct {
  char text[MAX_LEVEL_NAME_LENGTH];
  Levels_RootState *window_state;
  TuiObject *dir_up, *dir_down, *dir_left, *dir_rigth;
} Levels_ButtonState;

void levels_button_render_action(TuiObject *self, Screen *s){
  Levels_ButtonState *self_state = (Levels_ButtonState *)(self->state);
  const char *str = self_state->text;
  bool is_selected = (self == self_state->window_state->selected_child);
  int c = 0;
  for(int i = 0; i < self->size.x; ++i){
    tui_putc(self, (Cell) {
      .c = str[c] != '\0' ? str[c] : ' ',
      .background_color = is_selected ? COL_WHITE : COL_BLACK,
      .foreground_color = is_selected ? COL_BLACK : COL_WHITE
    }, i32vec2_create(i, 0), s);
    if(str[c] != '\0') c++;
  }
}

void button_on_event(TuiObject *self, TUI_Event event){
  if(event.type != EVENT_KEYPRESS) return;

  Levels_ButtonState *state = (Levels_ButtonState *)self->state;
  if(self != state->window_state->selected_child) return;

  switch (event.keypress.key) {
    case KEY_ENTER:{
      TUI_Event e = (TUI_Event) {
        .start_game.type = EVENT_STARTGAME,
      };
      strncpy(e.start_game.level_name, state->text, MAX_LEVEL_NAME_LENGTH);
      tui_events_push(e, self->p_event_system); 
      break;
    }
    case 'd':{
      TUI_Event e = (TUI_Event) {
        .scoreboard.type = EVENT_GOTOSCOREBOARD,
      };
      strncpy(e.scoreboard.level_name, state->text, MAX_LEVEL_NAME_LENGTH);
      tui_events_push(e, self->p_event_system); 
      break;
    }
    default:
      break;
  }

  if(event.keypress.key == KEY_ENTER){
  }
}
void shift_buttons_y_offset(TuiObject *level_list, int offset){
  for(int i = 0; i < darr_size(level_list->children); ++i){
    TuiObject **button = (TuiObject **) darr_get(level_list->children, i);
    (*button)->offset.y += offset;
  }
}

void window_root_on_event(TuiObject *self, TUI_Event event){
  Levels_RootState *window_state = (Levels_RootState *)self->state;
  TuiObject *curr_sel_button = (TuiObject *)window_state->selected_child;
  Levels_ButtonState *curr_sel_button_state = (Levels_ButtonState *)curr_sel_button->state;

  if(event.type == EVENT_KEYPRESS){
    switch(event.keypress.key){
      case(KEY_DOWN):
        if(curr_sel_button_state->dir_down != NULL)
          window_state->selected_child = curr_sel_button_state->dir_down;
      break;  
      case(KEY_UP):
        if(curr_sel_button_state->dir_up != NULL)
          window_state->selected_child = curr_sel_button_state->dir_up;
      break;
      default: break;
    }
  }

  if(window_state->level_list == NULL) assert(0 && "no good");

  //cursor went off screen towards bottom 
  int end_of_list_y = window_state->level_list->_global_offset.y + window_state->level_list->size.y;
  if(curr_sel_button->_global_offset.y >= end_of_list_y){
    shift_buttons_y_offset(window_state->level_list, -curr_sel_button->size.y);
  }
  //cursor went off screen towards top
  if(curr_sel_button->_global_offset.y < window_state->level_list->_global_offset.y){
    shift_buttons_y_offset(window_state->level_list, curr_sel_button->size.y);
  }
}

void w_levels_init(TuiObject *root, EventSystem *event_system){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(window_root, wc_root_render_action);
  Levels_RootState *window_root_state = tui_tuiobj_add_state(window_root, sizeof(Levels_RootState), NULL);
  tui_events_subscribe(window_root, window_root_on_event, event_system);

  //hint
  TuiObject *hint_select = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_select, "Valassz egy palyat:");
  tui_tuiobj_set_render_action(hint_select, wc_hint_render_action);

  TuiObject *hint_scoreboard = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_END,
    .offset = i32vec2_create(HINT_PADDING_X, -HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_scoreboard, "[D] Dicseglista");
  tui_tuiobj_set_render_action(hint_scoreboard, wc_hint_render_action);

  TuiObject *hint_start = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_END,
    .offset = i32vec2_create(0, -HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_start, "[ENTER] Jatek inditasa");
  tui_tuiobj_set_render_action(hint_start, wc_hint_render_action);

  TuiObject *hint_quit = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_END,
    .vertical_alignment = ALIGN_END,
    .offset = i32vec2_create(-HINT_PADDING_X, -HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_quit, "[Q] Kilepes");
  tui_tuiobj_set_render_action(hint_quit, wc_hint_render_action);

  //just a 'div' to render list in the middle of the screen
  TuiObject *level_list = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, LEVEL_LIST_PADDING),
    .size = i32vec2_create(10, window_root->size.y - LEVEL_LIST_PADDING * 2) 
  });
  window_root_state->level_list = level_list;

  //level_select_button entries
  FILE *f = fopen(".\\assets\\levels.txt", "r");
  if(f == NULL) assert(0 && "couldn't open assets/levels.txt");

  char line[MAX_LEVEL_NAME_LENGTH + 2];
  int row = 0;
  while(fgets(line, sizeof line, f) != NULL){
    line[strcspn(line, "\r\n")] = '\0';
    if(line[0] == '\0') continue;

    TuiObject *btn = tui_create_tuiobj(level_list, &(TuiObjectDimensions){
      .horizontal_alignment = ALIGN_START,
      .vertical_alignment = ALIGN_START,
      .offset = i32vec2_create(0, row),
      .size = i32vec2_create(MAX_LEVEL_NAME_LENGTH, 1)
    });
    Levels_ButtonState *bs = tui_tuiobj_add_state(btn, sizeof(Levels_ButtonState), NULL);
    bs->window_state = window_root_state;
    strncpy(bs->text, line, strcspn(line, "."));
    btn->size = i32vec2_create(strlen(bs->text), 1);

    tui_tuiobj_set_render_action(btn, levels_button_render_action);

    tui_events_subscribe(btn, button_on_event, event_system);
    row++;
  }

  //first button as default selected
  TuiObject *first_level = *((TuiObject **) darr_get(level_list->children, 0));
  if(first_level == NULL) return;
  ((Levels_RootState *)(window_root->state))->selected_child = first_level;

  //connecting buttons
  TuiObject *prev_button = *((TuiObject **)darr_get(level_list->children, 0));
  Levels_ButtonState *prev_btn_state = (Levels_ButtonState *)(prev_button->state);
  assert(prev_btn_state != NULL && "first button did not have a state in levels");
  prev_btn_state->dir_up = prev_button;

  for(int i = 1; i < darr_size(level_list->children); ++i){
    TuiObject *button = *((TuiObject **)darr_get(level_list->children, i));
    Levels_ButtonState *btn_state = (Levels_ButtonState *)(button->state);
    assert(btn_state != NULL && "on of the buttons did not have a state in levels");

    prev_btn_state->dir_down = button;
    btn_state->dir_up = prev_button;

    prev_button = button;
    prev_btn_state = btn_state;
  }
}