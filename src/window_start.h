#include "tui.h"
#include <vec2/vec2.h>
#include <string.h>

#define WSTART_TITLE_OFFSET (i32Vec2){ .x = 0, .y = 3}
#define WSTART_TITLE_SIZE (i32Vec2){ .x = 40, .y = 10}

#define WSTART_NAMEFIELD_OFFSET (i32Vec2){ .x = 0, .y = 0}
#define W1_NAMEFIELD_SIZE (i32Vec2){ .x = 15, .y = 1}

#define W1_HINT_OFFSET (i32Vec2){ .x = 0, .y = 2}
#define W1_HINT_SIZE (i32Vec2){ .x = 15, .y = 1}

void w_start_init(TuiObject *root){
  
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = root->size 
  },
  NULL);
  tui_debug_tree(root, 0);
  printf("------------------------------\n");

  TuiObject *title = tui_create_tuiobj(NULL, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = i32vec2_create(40, 10)
  },
  tui_renderaction_setbgtowhite);

  TuiObject *name_field = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(15, 1)
  },
  tui_renderaction_setbgtowhite);

  const char hint_text[] = "[Enter] a kezdéshez";
  TuiObject *hint = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 2),
    .size = i32vec2_create(strlen(hint_text)-1, 1)
  },
  tui_renderaction_setbgtowhite);
}