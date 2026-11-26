#ifndef WINDOW_WIN_H
#define WINDOW_WIN_H

#include <tui.h>

void w_win_init(TuiObject *root, EventSystem *event_system, char *player_name, char *level_name, unsigned int move_count);

#endif