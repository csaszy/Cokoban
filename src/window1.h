#include "tui.h"
#include <vec2/vec2.h>
#include <string.h>

#define W1_TITLE_OFFSET (i32Vec2){ .x = 0, .y = 3}
#define W1_TITLE_SIZE (i32Vec2){ .x = 40, .y = 10}

#define W1_NAMEFIELD_OFFSET (i32Vec2){ .x = 0, .y = 0}
#define W1_NAMEFIELD_SIZE (i32Vec2){ .x = 15, .y = 1}

#define W1_HINT_OFFSET (i32Vec2){ .x = 0, .y = 2}
#define W1_HINT_SIZE (i32Vec2){ .x = 15, .y = 1}

void window_init(TuiObject *root){
  TuiObject *title = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = W1_TITLE_OFFSET,
    .size = W1_TITLE_SIZE
  },
  tui_renderaction_setbgtowhite);

  TuiObject *name_field = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = W1_NAMEFIELD_OFFSET,
    .size = W1_NAMEFIELD_SIZE
  },
  tui_renderaction_setbgtowhite);

  const char hint_text[] = "[Enter] a kezdéshez";
  TuiObject *hint = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = W1_HINT_OFFSET,
    .size = i32vec2_create(strlen(hint_text)-1, 1)
  },
  NULL);
}