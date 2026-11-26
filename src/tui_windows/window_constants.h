#ifndef WINDOW_CONSTANTS_H
#define WINDOW_CONSTANTS_H

#include <tui.h>

#define MAX_PLAYER_NAME_LENGTH 20
#define MAX_LEVEL_NAME_LENGTH 20

#define HINT_PADDING_X 2
#define HINT_PADDING_Y 1

// hints
typedef struct{
  char *text;
} HintState;

void wc_root_render_action(TuiObject *self, Screen *s);
void wc_hint_render_action(TuiObject *self, Screen *s);
void wc_set_hint_text_helper(TuiObject *hint, char *text);

#endif