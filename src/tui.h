#ifndef TUI_H
#define TUI_H

#include <assert.h>

#include <econio/econio.h>
#include <vec2/vec2.h>
#include <string.h>
#include "darr.h"
#include "events.h"
#include "debugmalloc.h"

#define DEFAULT_CELL (Cell) {       \
  .background_color = COL_BLACK,    \
  .foreground_color = COL_WHITE,    \
  .c = ' '                          \
}                      

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
  void (*render_action)(struct TuiObject*, Screen *); //self pointer 
  void (*on_event)(struct TuiObject*, TUI_Event event);
  int state_size;
  void *state;
} TuiObject;

void tui_SET_ALL(TuiObject *obj, Screen *s, Cell c);

TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions);
void tui_tuiobj_set_render_action(TuiObject *obj, void (*render_action)(TuiObject *, Screen *));
void *tui_tuiobj_add_state(TuiObject *obj, int state_size);
void tui_attach_tuiobj(TuiObject *parent, TuiObject *obj);
void tui_free_tuiobj(TuiObject *tuiobj);
void tui_putc(TuiObject *obj, Cell cell, i32Vec2 pos, Screen *p_screen);
void tui_draw_diff(Screen *p_screen);
void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset, Screen *p_screen);
void tui_render(TuiObject *root, Screen *p_screen);
Screen *tui_create_screen(i32Vec2 size);
void tui_free_screen(Screen *screen);
void tui_debug_tree_print_sep(int level);
void tui_debug_tree(TuiObject *root, int level);

#endif