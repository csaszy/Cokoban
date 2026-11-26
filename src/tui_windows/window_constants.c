#include "window_constants.h"

void wc_root_render_action(TuiObject *self, Screen *s){
  for(int x = 0; x < self->size.x; x++){
    for(int y = 0; y < self->size.y; y++){
      tui_putc(self, (Cell) {
        .c = ' ',
        .background_color = COL_DARKGRAY,
        .foreground_color = COL_BLACK
      }, i32vec2_create(x, y), s);
    }
  }
}

void wc_hint_render_action(TuiObject *self, Screen *s){
  HintState *state = (HintState *)self->state; 
  if(state == NULL) return;
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
void wc_set_hint_text_helper(TuiObject *hint, char *text){
  HintState *hint_state = tui_tuiobj_add_state(hint, sizeof(HintState), NULL);
  hint_state->text = text;
  hint->size = i32vec2_create(strlen(text), 1);
}