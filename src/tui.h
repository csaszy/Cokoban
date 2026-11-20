#ifndef TUI_H
#define TUI_H

#include <assert.h>

#include <econio/econio.h>
#include <vec2/vec2.h>
#include <string.h>
#include "darr.h"
#include "debugmalloc.h"

typedef enum {
  ALIGN_START,
  ALIGN_CENTER,
  ALIGN_END
} TuiAlignment;

typedef struct TuiObjectDimensions{
  TuiAlignment horizontal_alignment;
  TuiAlignment vertical_alignment;
  i32Vec2 offset;
  i32Vec2 size;
} TuiObjectDimensions;

typedef struct TuiObject{
  struct TuiObject *parent;
  Darr *children;
  TuiAlignment horizontal_alignment;
  TuiAlignment vertical_alignment;
  i32Vec2 offset;
  i32Vec2 size;
  char *content; 
  void (*render)(void);
  void (*on_key_down)(struct TuiObject*, int);
  bool has_changed;
} TuiObject;

void tui_renderaction_DEFAULT_SETTINGS();
void tui_renderaction_setbgtowhite();
TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions, void (*render)(void));
void tui_set_content(TuiObject *obj, char *str);
void tui_free_tuiobj(TuiObject *tuiobj);
void tui_debug_tree(TuiObject *root, int level);
void tui_draw_content(TuiObject *obj, i32Vec2 pos);
void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset);
void tui_render(TuiObject *root);

#endif