#ifndef EVENTS_H
#define EVENTS_H

#include <econio/econio.h>
#include "constants.h"
#include "darr.h"

typedef enum TUI_EVENT_TYPE{
    EVENT_KEYPRESS,
    EVENT_GOTOLEVELSELECET,
    EVENT_GOTOSCOREBOARD,
    EVENT_STARTGAME,
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
} TUI_GoToScoreboard; 

typedef struct {
    TUI_EVENT_TYPE type;
    char level_name[MAX_LEVEL_NAME_LENGTH];
} TUI_StartGameEvent; 

typedef union {
    TUI_EVENT_TYPE type; 
    TUI_KeypressEvent keypress;
    TUI_GoToLevelSelectEvent level_select;
    TUI_GoToScoreboard scoreboard;
    TUI_StartGameEvent start_game;
} TUI_Event;

typedef struct{
    Darr *event_queue; 
} EventSystem;

EventSystem *tui_create_event_system();
void tui_free_event_system(EventSystem *event_system);

#endif