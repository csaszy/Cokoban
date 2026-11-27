#ifndef EVENTS_H
#define EVENTS_H

/**
 * @addtogroup tui_module 
 * @brief TUI eventek.
 * @{
 */

#include <econio/econio.h>

typedef enum TUI_EVENT_TYPE{
    EVENT_KEYPRESS,
    EVENT_FINISHED_START,
    EVENT_GOTOLEVELSELECET,
    EVENT_GOTOSCOREBOARD,
    EVENT_STARTGAME,
    EVENT_WONGAME
} TUI_EVENT_TYPE;

typedef struct {
    TUI_EVENT_TYPE type;
    EconioKey key;
} TUI_KeypressEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
    char *player_name;
} TUI_FinishedStartEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
} TUI_GoToLevelSelectEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
    char *level_name;
} TUI_GoToScoreboard; 

typedef struct {
    TUI_EVENT_TYPE type;
    char *level_name;
} TUI_StartGameEvent; 

typedef struct {
    TUI_EVENT_TYPE type;
    unsigned int move_count;
} TUI_WonGame; 

typedef union {
    TUI_EVENT_TYPE type; 
    TUI_KeypressEvent keypress;
    TUI_FinishedStartEvent finsihed_start;
    TUI_GoToLevelSelectEvent level_select;
    TUI_GoToScoreboard scoreboard;
    TUI_StartGameEvent start_game;
    TUI_WonGame won_game;
} TUI_Event;

typedef struct{
    Darr *event_queue; 
} EventSystem;

EventSystem *tui_create_event_system();
void tui_free_event_system(EventSystem *event_system);

/** @} */
#endif