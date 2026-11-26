#ifndef TUI_H
#define TUI_H

#include <assert.h>
#include <string.h>

#include <econio/econio.h>
#include <vec2/vec2.h>
#include "darr.h"
#include "events.h"


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
  void (*on_event)(struct TuiObject*, TUI_Event);
  EventSystem *p_event_system;
  int state_size;
  void *state;
  void (*state_free_action)(void *);
} TuiObject;

void tui_SET_ALL(TuiObject *obj, Screen *s, Cell c);

TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions);
void tui_free_tuiobj(TuiObject *tuiobj);
void *tui_tuiobj_add_state(TuiObject *obj, int state_size, void (*state_free_action)(void *));
void tui_attach_tuiobj(TuiObject *parent, TuiObject *obj);
void tui_remove_and_free_all_child(TuiObject *obj);

Screen *tui_create_screen(i32Vec2 size);
void tui_free_screen(Screen *screen);
void tui_putc(TuiObject *obj, Cell cell, i32Vec2 pos, Screen *p_screen);
int tui_puts(TuiObject *obj, char *str, Cell cell, i32Vec2 pos, Screen *p_screen);
void tui_draw_diff(Screen *p_screen);
void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset, Screen *p_screen);
void tui_render(TuiObject *root, Screen *p_screen);
void tui_tuiobj_set_render_action(TuiObject *obj, void (*render_action)(TuiObject *, Screen *));

void tui_events_subscribe(TuiObject *obj, void (*callback)(TuiObject*, TUI_Event), EventSystem *event_system);
void tui_events_push(TUI_Event event, EventSystem *event_system);
void _tui_events_pop(EventSystem *event_system);
void _tui_dispatch_event(TuiObject *root, TUI_Event event);
void tui_dispatch_events_in_queue(TuiObject *root, EventSystem *event_system);

void tui_debug_tree_print_sep(int level);
void tui_debug_tree(TuiObject *root, int level);

#endif