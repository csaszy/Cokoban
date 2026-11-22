#ifndef EVENTS_H
#define EVENTS_H

#include "darr.h"
#include "tui.h"

typedef enum{
    EVENT_KEYPRESS,
    EVENT_GOTOLEVELSELECET,
} TUI_EVENT_TYPE;

typedef struct {
    TUI_EVENT_TYPE type;
    char code;
} TUI_KeypressEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
    void *data;
} TUI_GoToLevelSelectEvent; 

typedef union {
    TUI_EVENT_TYPE type; 
    TUI_KeypressEvent keypress;
    TUI_GoToLevelSelectEvent level_select;
} TUI_Event;

void tui_event_queue_add(TUI_Event event);
void tui_event_queue_pop();
void tui_dispatch_events();

#endif