#include "tui.h"
#include <vec2/vec2.h>
#include <string.h>

#define MAX_LEVEL_NAME_LENGTH 20
#define LEVEL_LIST_PADDING 3
#define FIRST_HINT_STR "Valassz a palyak kozul:"

typedef struct {
  TuiObject *selected_child;
} Levels_RootState;

typedef struct {
  char text[MAX_LEVEL_NAME_LENGTH];
  Levels_RootState *window_state;
  TuiObject *dir_up, *dir_down, *dir_left, *dir_rigth;
} Levels_ButtonState;

void levels_root_render_action(TuiObject *self, Screen *s){
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

void levels_button_render_action(TuiObject *self, Screen *s){
  Levels_ButtonState *self_state = (Levels_ButtonState *)(self->state);
  const char *str = self_state->text;
  bool is_selected = is_selected = self == self_state->window_state->selected_child;
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

void levels_hint_render(TuiObject *self, Screen *s){
  const char *str = FIRST_HINT_STR;
  int c = 0;
  for(int i = 0; i < self->size.x; ++i){
    tui_putc(self, (Cell) {
      .c = str[c] != '\0' ? str[c] : ' ',
      .background_color = COL_WHITE,
      .foreground_color = COL_BLACK
    }, i32vec2_create(i, 0), s);
    if(str[c] != '\0') c++;
  }
}

void w_levels_init(TuiObject *root){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(window_root, levels_root_render_action);
  Levels_RootState *window_root_state = tui_tuiobj_add_state(window_root, sizeof(Levels_RootState));

  //hint
  TuiObject *hint = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(1, 1),
    .size = i32vec2_create(strlen(FIRST_HINT_STR), 1) 
  });
  tui_tuiobj_set_render_action(hint, levels_hint_render);

  //just a 'div' to render list in the middle of the screen
  TuiObject *level_list = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(2, LEVEL_LIST_PADDING),
    .size = i32vec2_create(8, window_root->size.y - LEVEL_LIST_PADDING * 2) 
  });

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
    Levels_ButtonState *bs = tui_tuiobj_add_state(btn, sizeof(Levels_ButtonState));
    bs->window_state = window_root_state;
    strncpy(bs->text, line, MAX_LEVEL_NAME_LENGTH - 1);
    tui_tuiobj_set_render_action(btn, levels_button_render_action);
    row++;
  }

  const char *back_button_str = "[Vissza]";
  TuiObject *back_button = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_END,
    .offset = i32vec2_create(1, -1),
    .size = i32vec2_create(strlen(back_button_str), 1) 
  });
  Levels_ButtonState *bs = tui_tuiobj_add_state(back_button, sizeof(Levels_ButtonState));
  bs->window_state = window_root_state;
  strncpy(bs->text, back_button_str, strlen(back_button_str));
  tui_tuiobj_set_render_action(back_button, levels_button_render_action);

  //first button as default selected
  TuiObject *first_level = *((TuiObject **) darr_get(level_list->children, 0));
  if(first_level == NULL) return;
  ((Levels_RootState *)(window_root->state))->selected_child = first_level;
  //tui_debug_tree(root, 0);
  //connecting buttons
}