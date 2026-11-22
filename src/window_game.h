#include "tui.h"
#include "constants.h"
#include "game.h"

void levels_root_render_action(TuiObject *self, Screen *s){
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

void w_levels_init(TuiObject *root, char *level_name){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(window_root, levels_root_render_action);
  
  TuiObject *game = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(0, 0)
  });

  Game_State *game_state = (Game_State *) tui_tuiobj_add_state(game, sizeof(Game_State), game_free_game_state);
  game_setup_game_state(level_name, game_state);
  game->size = game_state->size;

  
}