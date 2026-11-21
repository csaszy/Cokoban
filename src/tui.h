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

typedef struct{
  char c;
  int foreground_color;
  int background_color;
} Cell;

typedef struct{
  i32Vec2 size;
  Cell *prev_content;
  Cell *curr_content;
} Screen;

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
  i32Vec2 _global_offset;
  Screen *p_screen;
  void (*render)(struct TuiObject*); //self pointer 
  int state_size;
  void *state;
} TuiObject;

void tui_renderaction_DEFAULT_SETTINGS();
void tui_renderaction_setbgtowhite();
TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions, void (*render)(void));
void tui_attach_tuiobj(TuiObject *parent, TuiObject *obj);
void tui_set_content(TuiObject *obj, char *str);
void tui_free_tuiobj(TuiObject *tuiobj);
void tui_debug_tree(TuiObject *root, int level);
void tui_draw_content(TuiObject *obj, i32Vec2 pos);
void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset);
void tui_render(TuiObject *root);

#endif