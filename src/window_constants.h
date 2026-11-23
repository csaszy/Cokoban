#ifndef WINDOW_CONSTANTS_H
#define WINDOW_CONSTANTS_H

#include "tui.h"

// hints
typedef struct{
  char *text;
} HintState;

void wc_root_render_action(TuiObject *self, Screen *s);
void wc_hint_render_action(TuiObject *self, Screen *s);
void wc_set_hint_text_helper(TuiObject *hint, char *text);

#endif