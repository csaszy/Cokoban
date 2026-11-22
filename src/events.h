#ifndef EVENTS_H
#define EVENTS_H

#include <econio/econio.h>
#include "constants.h"

typedef enum TUI_EVENT_TYPE{
    EVENT_KEYPRESS,
    EVENT_GOTOLEVELSELECET,
    EVENT_LEVELSELECTED,
} TUI_EVENT_TYPE;

typedef struct {
    TUI_EVENT_TYPE type;
    EconioKey key;
} TUI_KeypressEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
    void *data;
} TUI_GoToLevelSelectEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
    char level_name[MAX_LEVEL_NAME_LENGTH];
} TUI_LevelSelectedEvent; 

typedef union {
    TUI_EVENT_TYPE type; 
    TUI_KeypressEvent keypress;
    TUI_GoToLevelSelectEvent level_select;
} TUI_Event;

#endif