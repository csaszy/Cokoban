#include "tui.h"
#include "constants.h"
#include "game.h"

#define HINT_PADDING 3

// hints
typedef struct{
  char *text;
} HintState;
void game_hint_render_action(TuiObject *self, Screen *s){
  HintState *state = (HintState *)self->state; 
  int c = 0;
  for(int i = 0; i < self->size.x; ++i){
    tui_putc(self, (Cell) {
      .c = state->text[c] != '\0' ? state->text[c] : ' ',
      .background_color = COL_DARKGRAY,
      .foreground_color = COL_BLACK
    }, i32vec2_create(i, 0), s);
    if(state->text[c] != '\0') c++;
  }
}
void game_set_hint_text_helper(TuiObject *hint, char *text){
  HintState *hint_state = tui_tuiobj_add_state(hint, sizeof(HintState), NULL);
  hint_state->text = text;
  hint->size = i32vec2_create(strlen(text), 1);
}

//window 
void game_root_render_action(TuiObject *self, Screen *s){
  for(int x = 0; x < self->size.x; x++){
    for(int y = 0; y < self->size.y; y++){
      tui_putc(self, (Cell) {
        .c = ' ',
        .foreground_color = COL_BLACK,
        .background_color = COL_DARKGRAY,
      }, i32vec2_create(x, y), s);
    }
  }
}
void game_root_on_event(TuiObject *self, TUI_Event event){
  if(event.type != EVENT_KEYPRESS) return;
  Game_State *game_state = (Game_State *)self->state;

  switch (event.keypress.key) {
    case KEY_ESCAPE:
      tui_events_push((TUI_Event){
        .level_select.type = EVENT_GOTOLEVELSELECET,
        .level_select.data = game_state->p_player_name
      }, self->p_event_system);
      break;
    case KEY_UP:
      game_move_player(DIR_UP, game_state);
      break;
    case KEY_DOWN:
      game_move_player(DIR_DOWN, game_state);
      break;
    case KEY_LEFT:
      game_move_player(DIR_LEFT, game_state);
      break;
    case KEY_RIGHT:
      game_move_player(DIR_RIGHT, game_state);
      break;
    case 'r':
      game_free_game_state_internals(game_state);
      game_setup_game_state(game_state, game_state->level_name, game_state->p_player_name); 
      break;
    default:
      break;
  }
}

//game-view
typedef struct{
  Game_State *game_state;
} GameViewState;
void game_game_view_render_action(TuiObject *self, Screen *s){
  GameViewState *self_state = (GameViewState *)self->state;
  Game_State *game_state = self_state->game_state;

  for(int y = 0; y < game_state->size.y; ++y){
    for(int x = 0; x < game_state->size.x; ++x){
      tui_putc(
        self, 
        (Cell){
          .c = game_state->map[y][x],
          .foreground_color = COL_WHITE,
          .background_color = COL_BLACK
        }, 
        i32vec2_create(x*2, y),
        s
      );
      tui_putc(
        self, 
        (Cell){
          .c = ' ',
          .foreground_color = COL_WHITE,
          .background_color = COL_BLACK
        }, 
        i32vec2_create(x*2 + 1, y),
        s
      );
    }
  }
}

void w_game_init(TuiObject *root, char *level_name, char *player_name, EventSystem *event_system){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(window_root, game_root_render_action);

  Game_State *game_state = (Game_State *) tui_tuiobj_add_state(window_root, sizeof(Game_State), game_free_game_state_internals);
  game_setup_game_state(game_state, level_name, player_name);

  tui_events_subscribe(window_root, game_root_on_event, event_system);
  
  //hints ----- 
  TuiObject *game_view = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(game_state->size.x * 2, game_state->size.y)
  });
  GameViewState *gvs = tui_tuiobj_add_state(game_view, sizeof(GameViewState), NULL);
  gvs->game_state = game_state;
  tui_tuiobj_set_render_action(game_view, game_game_view_render_action);

  TuiObject *hint1 = tui_create_tuiobj(
    window_root, 
    &(TuiObjectDimensions){
      .horizontal_alignment = ALIGN_START,
      .vertical_alignment = ALIGN_END,
      .offset = i32vec2_create(HINT_PADDING, -HINT_PADDING),
      .size = i32vec2_create(0, 0)
    }
  );
  game_set_hint_text_helper(hint1, "[R] restart");
  tui_tuiobj_set_render_action(hint1, game_hint_render_action);

  TuiObject *hint2 = tui_create_tuiobj(
    window_root,
    &(TuiObjectDimensions){
      .horizontal_alignment = ALIGN_CENTER,
      .vertical_alignment = ALIGN_END,
      .offset = i32vec2_create(-HINT_PADDING, -HINT_PADDING),
      .size = i32vec2_create(0, 0)
    }
  );
  game_set_hint_text_helper(hint2, " [ESC] vissza");
  tui_tuiobj_set_render_action(hint2, game_hint_render_action);

  TuiObject *hint3 = tui_create_tuiobj(
    window_root,
    &(TuiObjectDimensions){
      .horizontal_alignment = ALIGN_END,
      .vertical_alignment = ALIGN_END,
      .offset = i32vec2_create(-HINT_PADDING, -HINT_PADDING),
      .size = i32vec2_create(0, 0)
    }
  );
  game_set_hint_text_helper(hint3, "[Q] kilepes");
  tui_tuiobj_set_render_action(hint3, game_hint_render_action);
  
}