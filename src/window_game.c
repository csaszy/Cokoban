#include "tui.h"
#include "constants.h"
#include "game.h"
#include "window_constants.h"

#include "game.h"

//window 
void game_root_on_event(TuiObject *self, TUI_Event event){
  if(event.type != EVENT_KEYPRESS) return;
  Game_State *game_state = (Game_State *)self->state;

  switch ((int)event.keypress.key) {
    case KEY_ESCAPE:
      tui_events_push((TUI_Event){
        .level_select.type = EVENT_GOTOLEVELSELECET,
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
      game_setup_game_state(game_state, game_state->p_level_name, game_state->p_player_name); 
      break;
    case 'c': //cheat button
        tui_events_push((TUI_Event){
            .won_game.type = EVENT_WONGAME,
            .won_game.move_count = game_state->move_count
        }, self->p_event_system);
    default:
      break;
  }

  if(game_won(game_state)){
    tui_events_push((TUI_Event){
        .won_game.type = EVENT_WONGAME,
        .won_game.move_count = game_state->move_count
    }, self->p_event_system);
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
  tui_tuiobj_set_render_action(window_root, wc_root_render_action);

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
      .offset = i32vec2_create(HINT_PADDING_X, -HINT_PADDING_Y),
      .size = i32vec2_create(0, 0)
    }
  );
  wc_set_hint_text_helper(hint1, "[R] restart");
  tui_tuiobj_set_render_action(hint1, wc_hint_render_action);

  TuiObject *hint2 = tui_create_tuiobj(
    window_root,
    &(TuiObjectDimensions){
      .horizontal_alignment = ALIGN_CENTER,
      .vertical_alignment = ALIGN_END,
      .offset = i32vec2_create(0, -HINT_PADDING_Y),
      .size = i32vec2_create(0, 0)
    }
  );
  wc_set_hint_text_helper(hint2, "[ESC] vissza");
  tui_tuiobj_set_render_action(hint2, wc_hint_render_action);

  TuiObject *hint3 = tui_create_tuiobj(
    window_root,
    &(TuiObjectDimensions){
      .horizontal_alignment = ALIGN_END,
      .vertical_alignment = ALIGN_END,
      .offset = i32vec2_create(-HINT_PADDING_X, -HINT_PADDING_Y),
      .size = i32vec2_create(0, 0)
    }
  );
  wc_set_hint_text_helper(hint3, "[Q] kilepes");
  tui_tuiobj_set_render_action(hint3, wc_hint_render_action);
  
}